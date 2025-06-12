#pragma once
#include <ShlObj.h>
#include <XorStr.h>

#include <filesystem>
#include <string>

#include "HarmonySans.h"
#include "ImGui/imgui.h"
#include "Functions/config.h"

static constexpr ImWchar basic_ranges[] = {

    0x0020, 0x00FF,  // ASCII + Latin-1 Supplement
    0x0102, 0x0103, 0x0110, 0x0111, 0x0128, 0x0129, 0x0168,
    0x0169, 0x01A0, 0x01A1, 0x01AF, 0x01B0,

    0x0400, 0x052F, 0x2DE0, 0x2DFF, 0xA640, 0xA69F,

    0x2000, 0x206F, 0xFF00, 0xFFEF, 0xFFFD, 0xFFFD, 0};

static constexpr ImWchar greek_ranges[] = {0x0370, 0x03FF, 0};

static constexpr ImWchar vietnamese_ranges[] = {0x1EA0, 0x1EF9, 0};

static constexpr ImWchar kanji_ranges[] = {
    0x3000, 0x30FF, 0x31F0, 0x31FF, 0xFF00, 0xFFEF, 0x4e00, 0x9FAF, 0,
};

static constexpr ImWchar korean_ranges[] = {0x3131, 0x3163,  // Korean alphabets
                                            0xAC00,
                                            0xD7A3,  // Korean characters
                                            0};
static constexpr ImWchar arab_ranges[] = {0x0600, 0x06FF, 0};
static constexpr ImWchar thai_ranges[] = {0x0E00, 0x0E7F,  // Thai
                                          0};

class FontManager {
 public:

  inline void ReloadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    ImFontConfig base_cfg;
    base_cfg.FontDataOwnedByAtlas = false;
    ImFont* base_font =
        io.Fonts->AddFontFromMemoryTTF((void*)harmonySans, sizeof(harmonySans),
                                       16.0f, &base_cfg, basic_ranges);

    ImFontConfig merge_cfg;
    merge_cfg.MergeMode = true;
    merge_cfg.FontDataOwnedByAtlas = false;

    if (LangSettings::greek) {
      io.Fonts->AddFontFromMemoryTTF((void*)harmonySans, sizeof(harmonySans),
                                     16.0f, &merge_cfg,
                                     greek_ranges);
    }
    if (LangSettings::vietnamese) {
      io.Fonts->AddFontFromMemoryTTF((void*)harmonySans, sizeof(harmonySans),
                                     16.0f, &merge_cfg,
                                     vietnamese_ranges);
    }
    if (LangSettings::kanji) {
      io.Fonts->AddFontFromMemoryTTF((void*)harmonySans, sizeof(harmonySans),
                                     16.0f, &merge_cfg,
                                     kanji_ranges);
    }
    if (LangSettings::korean) {
      AddFontWithRange(merge_cfg, "malgun.ttf", korean_ranges);
    }
    if (LangSettings::arabic) {
      AddFontWithRange(merge_cfg, "dubai-regular.ttf", arab_ranges);
    }
    if (LangSettings::thai) {
      AddFontWithRange(merge_cfg, "leelawui.ttf", thai_ranges);
    }

    io.Fonts->Build();
    ImGui_ImplDX9_InvalidateDeviceObjects();
    ImGui_ImplDX9_CreateDeviceObjects();
    global::fontUpdatePending = false;
  }

 private:
  inline std::string GetSystemFontPath() {
    char path[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, path);
    return std::string(path) + "/";
  }

  inline void AddFontWithRange(ImFontConfig& cfg, const char* fontName,
                        const ImWchar* ranges) {
    const std::string fullPath = GetSystemFontPath() + fontName;
    if (!std::filesystem::exists(fullPath)) {
      MessageBoxA(nullptr, XorStr("Font file missing."),
                  XorStr("Samidare Error"), MB_ICONERROR);
      return;
    }
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(fullPath.c_str(), 16.0f, &cfg, ranges);
  }
};

static FontManager font_manager;