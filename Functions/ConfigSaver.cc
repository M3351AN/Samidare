#include "ConfigSaver.h"

#include <XorStr.h>

#include <unordered_map>

#include "../global.h"

namespace MyConfigSaver {

static std::string GetAuthorFromFile(const std::string& filePath) {
  std::ifstream file(filePath);
  std::string line;
  for (int i = 0; i < 3 && std::getline(file, line); ++i) {
    if (i == 2 && line.find(XorStr("# Author: ")) == 0) {
      return line.substr(10);
    }
  }
  return "";
}

void SaveConfig(const std::string& filename, const std::string& author) {
  const std::string fullPath = config::path + XorStr("/") + filename;
  const bool isNewFile = !std::filesystem::exists(fullPath);

  std::string actualAuthor = author;

  if (isNewFile) {
    if (actualAuthor.empty()) {
      if (const char* username = global::userName.c_str()) {
        actualAuthor = username;
      }
    }
  } else {
    actualAuthor = GetAuthorFromFile(fullPath);
  }

  YAML::Emitter emitter;
  emitter << YAML::Comment(
      XorStr("Samidare Configuration File\nVersion: 1.0\nAuthor: ") +
      actualAuthor);
  emitter << YAML::BeginMap;

  emitter << YAML::Key << XorStr("config") << YAML::Value << YAML::BeginMap;
#define X(var, type) emitter << YAML::Key << #var << YAML::Value << config::var;
  AUTO_CONFIG_VARS
#undef X
  emitter << YAML::EndMap;

  emitter << YAML::EndMap;

  std::ofstream configFile(fullPath);
  if (!configFile.is_open()) {
    MessageBoxA(nullptr, (XorStr("Could not open file:") + fullPath).c_str(),
                XorStr("Samidare"), MB_OK);
    return;
  }
  configFile << emitter.c_str();
  configFile.close();

  printf(XorStr("Configuration %s at %s (Author: %s)\n"),
         isNewFile ? XorStr("created") : XorStr("updated"), fullPath.c_str(),
         actualAuthor.c_str());
}

void LoadConfig(const std::string& filename) {
  YAML::Node root = YAML::LoadFile(config::path + XorStr("/") + filename);
  if (root["config"]) {
    YAML::Node configNode = root["config"];
#define X(var, type)                           \
  if (configNode[#var]) {                      \
    config::var = configNode[#var].as<type>(); \
  }
    AUTO_CONFIG_VARS
#undef X
  }
}

#ifdef _DEBUG
void ExportLangs() {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "translate" << YAML::Value << YAML::BeginMap;

#define Y(var, type) \
  out << YAML::Key << #var << YAML::Value << LangSettings::var;

  AUTO_LANG_VARS

#undef Y

  out << YAML::EndMap;
  out << YAML::EndMap;

  std::ofstream fout(LangSettings::path + XorStr("/") +
                     "language_template.yaml");
  fout << out.c_str();
}
#endif

void ExportDefaultLang() {
  YAML::Emitter out;
  out << YAML::Comment(
      XorStr("This is Samidare default language file\nDo NOT edit this unless "
             "you renamed this file."));
  out << YAML::BeginMap;
  out << YAML::Key << "translate" << YAML::Value << YAML::BeginMap;

#define Y(var, type) \
  out << YAML::Key << #var << YAML::Value << LangSettings::var;

  AUTO_LANG_VARS

#undef Y

  out << YAML::EndMap;
  out << YAML::EndMap;

  std::ofstream fout(LangSettings::path + XorStr("/") +
                     "Default.yaml");
  fout << out.c_str();
}

void LoadLangs(const std::string& filename) {
  YAML::Node root = YAML::LoadFile(LangSettings::path + XorStr("/") + filename);
  if (root["translate"]) {
    YAML::Node configNode = root["translate"];
#define Y(var, type)                                 \
  if (configNode[#var]) {                            \
    LangSettings::var = configNode[#var].as<type>(); \
  }
    AUTO_LANG_VARS
#undef Y
  }
  global::fontUpdatePending = true;
}

void UpdateLangsFileList() {
  langsFileList.clear();
  std::error_code ec;

  for (const auto& entry :
       std::filesystem::directory_iterator(LangSettings::path, ec)) {
    if (ec) {
      MessageBoxA(nullptr, XorStr("Fetch language files error."),
                  XorStr("Samidare Error"), MB_ICONERROR);
      return;
    }

    if (!entry.is_regular_file() || entry.path().extension() != XorStr(".yaml"))
      continue;
    std::string langname = entry.path().filename().string();
    size_t pos = langname.find_last_of('.');
    if (pos != std::string::npos) {
      langname.erase(pos);
    }
    langsFileList.push_back(langname);
  }
}

void RenderLangsFileCombo() {
  UpdateLangsFileList();

  const char* currentItem = selectedLangsFile.empty()
                                ? XorStr("English")
                                : selectedLangsFile.c_str();

  if (ImGui::BeginCombo(XorStr("Language"), currentItem)) {
    for (const auto& file : langsFileList) {
      bool is_selected = (selectedLangsFile == file);
      if (ImGui::Selectable(file.c_str(), is_selected)) {
        selectedLangsFile = file;
        LoadLangs(selectedLangsFile + XorStr(".yaml"));
        ImGui::SaveIniSettingsToDisk(ImGui::GetIO().IniFilename);
      }
      if (is_selected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
#ifdef _DEBUG
  if (ImGui::Button(XorStr("Export Current Language Template"))) {
    ExportLangs();
  }
#endif
}
}  // namespace MyConfigSaver
