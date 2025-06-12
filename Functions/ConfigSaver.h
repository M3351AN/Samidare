#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "../ImGui/imgui.h"
#include "../Utils/XorStr.h"
#include "../Utils/yaml-cpp/yaml.h"
#include "config.h"

struct ConfigFileCache {
  std::filesystem::file_time_type modifyTime;
  std::string author;
  std::string modiTimeStr;
};

static std::string selectedConfigFile;
static std::string deletePendingFile;
static std::vector<std::tuple<std::string, std::string, std::string>>
    configFiles;
static std::unordered_map<std::string, ConfigFileCache> configFileCache;
static std::filesystem::file_time_type lastConfigScanTime;
static std::vector<std::string> langsFileList;

namespace MyConfigSaver {
inline std::string selectedLangsFile;
inline void UpdateConfigFiles() {
  static std::vector<std::string> currentFiles;
  currentFiles.clear();

  auto currentScanTime = std::filesystem::last_write_time(config::path);
  if (currentScanTime <= lastConfigScanTime && !configFiles.empty() &&
      deletePendingFile.empty() && selectedConfigFile.empty()) {
    return;
  }
  lastConfigScanTime = currentScanTime;

  std::vector<std::tuple<std::string, std::string, std::string>> newFiles;
  std::error_code ec;
  auto dirIter = std::filesystem::directory_iterator(config::path, ec);
  if (ec) return;

  newFiles.reserve(configFiles.size() * 2);

  for (const auto& entry : dirIter) {
    if (!entry.is_regular_file() || entry.path().extension() != XorStr(".yaml"))
      continue;

    const auto filename = entry.path().filename().string();
    currentFiles.push_back(filename);

    const auto modifyTime = entry.last_write_time();
    auto& cache = configFileCache[filename];

    if (cache.modifyTime == modifyTime && !cache.modiTimeStr.empty()) {
      newFiles.emplace_back(filename, cache.modiTimeStr, cache.author);
      continue;
    }

    std::string author;
    if (std::ifstream file{entry.path()}) {
      std::string line;
      for (int i = 0; i < 3 && std::getline(file, line); ++i) {
        if (i == 2 && line.find(XorStr("# Author: ")) == 0) {
          author = line.substr(10);
          break;
        }
      }
    }

    const auto sysTime =
        std::chrono::clock_cast<std::chrono::system_clock>(modifyTime);
    const std::time_t cTime = std::chrono::system_clock::to_time_t(sysTime);
    char timeBuf[26];
    ctime_s(timeBuf, sizeof(timeBuf), &cTime);
    std::string timeStr = timeBuf;
    if (!timeStr.empty() && timeStr.back() == '\n') {
      timeStr.pop_back();
    }

    cache = {modifyTime, author, timeStr};
    newFiles.emplace_back(filename, std::move(timeStr), std::move(author));
  }

  for (auto it = configFileCache.begin(); it != configFileCache.end();) {
    if (std::find(currentFiles.begin(), currentFiles.end(), it->first) ==
        currentFiles.end()) {
      it = configFileCache.erase(it);
    } else {
      ++it;
    }
  }

  std::sort(newFiles.begin(), newFiles.end(), [](const auto& a, const auto& b) {
    return std::get<0>(a) > std::get<0>(b);
  });

  configFiles.swap(newFiles);
}

static std::string GetAuthorFromFile(const std::string& filePath);

void SaveConfig(const std::string& filename, const std::string& author);

void LoadConfig(const std::string& filename);

#ifdef _DEBUG
void ExportLangs();
#endif
void ExportDefaultLang();

void LoadLangs(const std::string& filename);

void UpdateLangsFileList();

void RenderLangsFileCombo();

template <typename T>
inline static T ReadData(const YAML::Node& node, T defaultValue) {
  return node.IsDefined() ? node.as<T>() : defaultValue;
}
inline static int ReadOffset(const YAML::Node& node, int defaultValue) {
  return node.IsDefined() ? std::stoi(node.as<std::string>(), nullptr, 16)
                          : defaultValue;
}
inline static uint32_t ImColorToUInt32(const ImColor& color) {
  uint32_t r = static_cast<uint32_t>(color.Value.x * 255);
  uint32_t g = static_cast<uint32_t>(color.Value.y * 255) << 8;
  uint32_t b = static_cast<uint32_t>(color.Value.z * 255) << 16;
  uint32_t a = static_cast<uint32_t>(color.Value.w * 255) << 24;

  return r | g | b | a;
}

inline static ImColor UInt32ToImColor(uint32_t value) {
  ImColor TempColor;
  TempColor.Value.x = static_cast<float>(value & 0xFF) / 255.0f;
  TempColor.Value.y = static_cast<float>((value >> 8) & 0xFF) / 255.0f;
  TempColor.Value.z = static_cast<float>((value >> 16) & 0xFF) / 255.0f;
  TempColor.Value.w = static_cast<float>((value >> 24) & 0xFF) / 255.0f;
  return TempColor;
}

inline static std::vector<int> LoadVector(const YAML::Node& node,
                                          std::vector<int> defaultValue) {
  if (node.IsDefined() && node.IsSequence()) {
    std::vector<int> result;
    for (const YAML::Node& element : node) {
      result.push_back(element.as<int>());
    }
    return result;
  } else
    return defaultValue;
}

}  // namespace MyConfigSaver
