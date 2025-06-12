#include <string>

#include "../CS2x64.h"
#include "../UkiaStuff.h"
#include "../global.h"
#include "ConfigSaver.h"
static bool checkbox1;
static bool checkbox2;
static bool checkbox3;
static bool checkbox4;
static bool checkbox5;

inline void InitHitboxList() {

  auto HitboxList = config::HitboxList;

  auto it = std::find(HitboxList.begin(), HitboxList.end(), BONEINDEX::head);
  if (it != HitboxList.end()) checkbox1 = true;
  else
    checkbox1 = false;

  it = std::find(HitboxList.begin(), HitboxList.end(), BONEINDEX::neck_0);
  if (it != HitboxList.end()) checkbox2 = true;
  else
    checkbox2 = false;
  it = std::find(HitboxList.begin(), HitboxList.end(), BONEINDEX::spine_1);
  if (it != HitboxList.end()) checkbox3 = true;
  else
    checkbox3 = false;
  it = std::find(HitboxList.begin(), HitboxList.end(), BONEINDEX::spine_2);
  if (it != HitboxList.end()) checkbox4 = true;
  else
    checkbox4 = false;
  it = std::find(HitboxList.begin(), HitboxList.end(), BONEINDEX::pelvis);
  if (it != HitboxList.end()) checkbox5 = true;
  else
    checkbox5 = false;
}
inline void addHitbox(int BoneIndex) {
  config::HitboxList.push_back(BoneIndex);
}
inline void removeHitbox(int BoneIndex) {
  for (auto it = config::HitboxList.begin(); it != config::HitboxList.end();
       ++it) {
    if (*it == BoneIndex) {
      config::HitboxList.erase(it);
      break;
    }
  }
}
namespace Menu {

inline void Lumine() {
  ImGuiStyle& style = ImGui::GetStyle();

  style.Colors[ImGuiCol_Text] = ImColor(45, 35, 30, 255);
  style.Colors[ImGuiCol_TextDisabled] = ImColor(160, 130, 100, 255);

  style.Colors[ImGuiCol_WindowBg] = ImColor(241, 230, 221, 245);
  style.Colors[ImGuiCol_ChildBg] = ImColor(252, 239, 233, 245);
  style.Colors[ImGuiCol_PopupBg] = ImColor(239, 229, 209, 245);

  style.Colors[ImGuiCol_Border] = ImColor(106, 164, 196, 250);
  style.Colors[ImGuiCol_BorderShadow] = ImColor(161, 224, 245, 255);

  style.Colors[ImGuiCol_FrameBg] = ImColor(229, 219, 211, 255);
  style.Colors[ImGuiCol_FrameBgHovered] = ImColor(220, 210, 200, 255);
  style.Colors[ImGuiCol_FrameBgActive] = ImColor(148, 197, 220, 250);

  style.Colors[ImGuiCol_TitleBg] = ImColor(255, 240, 220, 200);
  style.Colors[ImGuiCol_TitleBgActive] = ImColor(255, 235, 210, 255);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(255, 240, 220, 200);

  style.Colors[ImGuiCol_MenuBarBg] = ImColor(245, 230, 215, 255);

  style.Colors[ImGuiCol_ScrollbarBg] = ImColor(240, 230, 220, 255);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(158, 207, 229, 250);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(148, 197, 219, 250);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(138, 187, 209, 250);

  style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);

  style.Colors[ImGuiCol_SliderGrab] = ImColor(205, 225, 235, 255);
  style.Colors[ImGuiCol_SliderGrabActive] = ImColor(195, 215, 225, 255);

  style.Colors[ImGuiCol_Button] = ImColor(216, 176, 132, 225);
  style.Colors[ImGuiCol_ButtonHovered] = ImColor(208, 187, 135, 225);
  style.Colors[ImGuiCol_ButtonActive] = ImColor(192, 149, 111, 225);

  style.Colors[ImGuiCol_Header] = ImColor(235, 208, 185, 255);
  style.Colors[ImGuiCol_HeaderHovered] = ImColor(220, 193, 177, 255);
  style.Colors[ImGuiCol_HeaderActive] = ImColor(211, 185, 168, 255);

  style.Colors[ImGuiCol_Separator] = ImColor(210, 190, 160, 255);
  style.Colors[ImGuiCol_SeparatorHovered] = ImColor(200, 180, 150, 255);
  style.Colors[ImGuiCol_SeparatorActive] = ImColor(190, 170, 140, 255);

  style.Colors[ImGuiCol_ResizeGrip] = ImColor(200, 180, 160, 255);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(190, 170, 150, 255);
  style.Colors[ImGuiCol_ResizeGripActive] = ImColor(180, 160, 140, 255);

  style.Colors[ImGuiCol_Tab] = ImColor(245, 215, 185, 255);
  style.Colors[ImGuiCol_TabHovered] = ImColor(235, 205, 175, 255);
  style.Colors[ImGuiCol_TabActive] = ImColor(225, 195, 165, 255);
  style.Colors[ImGuiCol_TabUnfocused] = ImColor(250, 230, 210, 255);
  style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(240, 220, 200, 255);

  style.Colors[ImGuiCol_PlotLines] = ImColor(120, 100, 80, 255);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImColor(140, 120, 100, 255);
  style.Colors[ImGuiCol_PlotHistogram] = ImColor(180, 150, 120, 255);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(200, 170, 140, 255);

  style.Colors[ImGuiCol_TableHeaderBg] = ImColor(235, 225, 215, 255);
  style.Colors[ImGuiCol_TableBorderStrong] = ImColor(210, 190, 160, 255);
  style.Colors[ImGuiCol_TableBorderLight] = ImColor(240, 230, 220, 255);
  style.Colors[ImGuiCol_TableRowBg] = ImColor(255, 245, 235, 255);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImColor(250, 235, 225, 255);

  style.Colors[ImGuiCol_TextSelectedBg] = ImColor(230, 200, 150, 150);

  style.Colors[ImGuiCol_DragDropTarget] = ImColor(255, 220, 100, 200);

  style.Colors[ImGuiCol_NavHighlight] = ImColor(175, 210, 235, 255);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImColor(255, 240, 220, 180);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImColor(200, 180, 160, 50);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImColor(200, 180, 160, 100);
}

inline void Style_AimStar() {
  ImGuiStyle& style = ImGui::GetStyle();

  style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.99999f, 0.99999f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] =
      ImVec4(0.690987f, 0.690980f, 0.690980f, 1.0f);
  style.Colors[ImGuiCol_WindowBg] =
      ImVec4(0.345098f, 0.219608f, 0.0f, 0.896400f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.502146f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.690987f);
  style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.800000f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_FrameBg] =
      ImVec4(0.545064f, 0.421080f, 0.0f, 0.502146f);
  style.Colors[ImGuiCol_FrameBgHovered] =
      ImVec4(1.0f, 0.999996f, 0.99999f, 0.8964f);
  style.Colors[ImGuiCol_FrameBgActive] =
      ImVec4(0.499142f, 0.326123f, 0.1989f, 0.780392f);
  style.Colors[ImGuiCol_TitleBg] =
      ImVec4(0.824034f, 0.615373f, 0.0f, 0.788235f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.695278f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] =
      ImVec4(1.0f, 0.695279f, 0.0f, 0.75f);
  style.Colors[ImGuiCol_MenuBarBg] =
      ImVec4(0.200000f, 0.219608f, 0.266667f, 0.65f);
  style.Colors[ImGuiCol_ScrollbarBg] =
      ImVec4(0.622318f, 0.432683f, 0.0f, 0.630901f);
  style.Colors[ImGuiCol_ScrollbarGrab] =
      ImVec4(1.0f, 0.999996f, 0.99999f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.875537f, 0.875528f, 0.875532f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] =
      ImVec4(0.678112f, 0.678105f, 0.678108f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.99999f, 0.999995f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.618026f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.721030f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.892857f, 0.0f, 0.75f);
  style.Colors[ImGuiCol_ButtonHovered] =
      ImVec4(0.996463f, 1.0f, 0.175966f, 0.75f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.669528f, 0.0f, 0.75f);
  style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.515021f, 0.0f, 0.785408f);
  style.Colors[ImGuiCol_HeaderHovered] =
      ImVec4(1.0f, 0.643114f, 0.266094f, 0.785408f);
  style.Colors[ImGuiCol_HeaderActive] =
      ImVec4(0.836910f, 0.429642f, 0.0f, 0.784314f);
  style.Colors[ImGuiCol_Separator] =
      ImVec4(0.261803f, 0.261802f, 0.261800f, 0.5f);
  style.Colors[ImGuiCol_SeparatorHovered] =
      ImVec4(0.098039f, 0.438896f, 0.749020f, 0.78f);
  style.Colors[ImGuiCol_SeparatorActive] =
      ImVec4(0.098039f, 0.400000f, 0.749020f, 1.0f);
  style.Colors[ImGuiCol_ResizeGrip] =
      ImVec4(0.466667f, 0.768627f, 0.827451f, 0.04f);
  style.Colors[ImGuiCol_ResizeGripHovered] =
      ImVec4(0.454902f, 0.196078f, 0.298039f, 1.0f);
  style.Colors[ImGuiCol_ResizeGripActive] =
      ImVec4(0.454902f, 0.196078f, 0.298039f, 1.0f);
  style.Colors[ImGuiCol_Tab] = ImVec4(1.0f, 0.360515f, 0.0f, 0.862000f);
  style.Colors[ImGuiCol_TabHovered] =
      ImVec4(0.991416f, 0.518159f, 0.251045f, 0.862745f);
  style.Colors[ImGuiCol_TabActive] =
      ImVec4(0.716738f, 0.258586f, 0.0f, 0.862745f);
  style.Colors[ImGuiCol_TabUnfocused] =
      ImVec4(0.066667f, 0.101961f, 0.145098f, 0.972400f);
  style.Colors[ImGuiCol_TabUnfocusedActive] =
      ImVec4(0.133333f, 0.258824f, 0.423529f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] =
      ImVec4(0.858824f, 0.929412f, 0.886275f, 0.63f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.197425f, 1.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] =
      ImVec4(0.858824f, 0.929412f, 0.886275f, 0.63f);
  style.Colors[ImGuiCol_PlotHistogramHovered] =
      ImVec4(0.454902f, 0.196078f, 0.298039f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] =
      ImVec4(0.201715f, 0.201715f, 0.201717f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] =
      ImVec4(0.403429f, 0.403429f, 0.403434f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] =
      ImVec4(9.9999e-07f, 9.9999e-07f, 9.999999e-07f, 0.0f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] =
      ImVec4(9.9999e-07f, 9.9999e-07f, 9.9999e-07f, 0.223176f);
  style.Colors[ImGuiCol_TextSelectedBg] =
      ImVec4(0.729614f, 0.729606f, 0.729610f, 0.43f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
  style.Colors[ImGuiCol_NavHighlight] =
      ImVec4(0.258824f, 0.588235f, 0.976471f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}

inline void DrawMenu() {
  switch (config::Style) {
    case 0:
      Lumine();
      break;
    case 1:
      Style_AimStar();
      break;
    case 2:
      ImGui::StyleColorsClassic();
      break;
    case 3:
      ImGui::StyleColorsDark();
      break;
    case 4:
      ImGui::StyleColorsLight();
      break;
    default:
      Lumine();
  }
  float textoffset;
  int randomresult = (rand() % 15) + 1;
  if (randomresult % 2 == 0)
    textoffset = randomresult * -0.1f;
  else
    textoffset = randomresult * 0.1f;
  ImGuiIO& io = ImGui::GetIO();
  ImGuiStyle& style = ImGui::GetStyle();
  if (!config::ShowMenu) return;
  const ImVec2 vecScreenSize = io.DisplaySize;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(720, 365));

  ImGui::SetNextWindowPos(
      ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), ImGuiCond_Once,
      ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(720, 370), ImGuiCond_Always);
  ImGui::Begin(XorStr("Samidare for Counter-Strike 2"), NULL,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoCollapse);

  const ImVec2 vecMenuPos = ImGui::GetWindowPos();
  const ImVec2 vecMenuSize = ImGui::GetWindowSize();
  ImDrawList* pDrawList = ImGui::GetWindowDrawList();

  ImGui::BeginTabBar(XorStr("##Tabs"));
#ifdef _DEBUG
  if (ImGui::BeginTabItem(XorStr("Semi-rage"))) {
    ImGui::Text(XorStr("Coming soon?"));
    ImGui::EndTabItem();
  }
#endif
  if (ImGui::BeginTabItem(LangSettings::TabLegit.c_str())) {
    ImVec2 child_size =
        ImVec2((ImGui::GetColumnWidth() - (style.ItemSpacing.x * 2)) / 3,
               ImGui::GetWindowHeight() - 10.f -
                   (ImGui::GetCursorPosY() + style.ItemInnerSpacing.y * 2));
    auto childBegin = ImGui::GetCursorPosY();

    ImGui::SetCursorPosY(childBegin);
    ImGui::BeginChild(LangSettings::ChildAimAssist.c_str(), child_size);
    {
        ImGui::Checkbox(LangSettings::SwitchEnableAimBot.c_str(), &config::AimBot);
        ImGui::HotKey(XorStr(" ##hotkeyaim"),&config::AimBotHotKey);
        ImGui::SliderFloat(LangSettings::SliderAimFov.c_str(), &config::AimFov, 0.1f, 45.f, XorStr("%.2f"));
        ImGui::SliderFloat(LangSettings::SliderAimFovMin.c_str(), &config::AimFovMin, 0.f, 1.f, XorStr("%.2f"));
        ImGui::SliderFloat(LangSettings::SliderAimSmooth.c_str(), &config::AimSmooth, 2.f, 100.f, XorStr("%.1f"));
        ImGui::SliderFloat(LangSettings::SliderAimMaxRecoil.c_str(),&config::AimMaxRecoil, 1.f, 90.f, XorStr("%.1f"));
        ImGui::Checkbox(LangSettings::SwitchAimScopeOnly.c_str(), &config::AimScopeOnly);
        ImGui::Checkbox(LangSettings::SwitchAimIgnoreFlash.c_str(), &config::AimIgnoreFlash);
        ImGui::SliderInt(LangSettings::SliderAimBullet.c_str(), &config::AimBullet, 0, 5, XorStr("%d"));
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::SetCursorPosY(childBegin);
    ImGui::BeginChild(LangSettings::ChildHitBox.c_str(), child_size);
    {
      InitHitboxList();
      ImVec2 StartPos = ImGui::GetCursorScreenPos();
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Image((ImTextureID)global::Zekamashi, ImVec2{192, 260},
                   ImVec2{0, 0}, ImVec2{1, 1});
      ImGui::GetWindowDrawList()->AddLine(
          ImVec2(StartPos.x + 100, StartPos.y + 55),
          ImVec2(StartPos.x + 190, StartPos.y + 55),
          ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)), 1.8f);  // Head
      ImGui::SetCursorScreenPos(ImVec2(StartPos.x + 190, StartPos.y + 55));
      if (ImGui::Checkbox(XorStr("###Head"), &checkbox1)) {
        if (checkbox1) {
          addHitbox(BONEINDEX::head);
        } else {
          removeHitbox(BONEINDEX::head);
        }
      }
      ImGui::GetWindowDrawList()->AddLine(
          ImVec2(StartPos.x + 100, StartPos.y + 85),
          ImVec2(StartPos.x + 15, StartPos.y + 85),
          ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)), 1.8f);  // Neck
      ImGui::SetCursorScreenPos(ImVec2(StartPos.x + 5, StartPos.y + 85));
      if (ImGui::Checkbox(XorStr("###Neck"), &checkbox2)) {
        if (checkbox2) {
          addHitbox(BONEINDEX::neck_0);
        } else {
          removeHitbox(BONEINDEX::neck_0);
        }
      }
      ImGui::GetWindowDrawList()->AddLine(
          ImVec2(StartPos.x + 90, StartPos.y + 110),
          ImVec2(StartPos.x + 190, StartPos.y + 110),
          ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)), 1.8f);  // Chest
      ImGui::SetCursorScreenPos(ImVec2(StartPos.x + 190, StartPos.y + 110));
      if (ImGui::Checkbox(XorStr("###Chest"), &checkbox3)) {
        if (checkbox3) {
          addHitbox(BONEINDEX::spine_1);
        } else {
          removeHitbox(BONEINDEX::spine_1);
        }
      }
      ImGui::GetWindowDrawList()->AddLine(
          ImVec2(StartPos.x + 80, StartPos.y + 135),
          ImVec2(StartPos.x + 15, StartPos.y + 135),
          ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)), 1.8f);  // Stomache
      ImGui::SetCursorScreenPos(ImVec2(StartPos.x + 5, StartPos.y + 135));
      if (ImGui::Checkbox(XorStr("###Stomache"), &checkbox4)) {
        if (checkbox4) {
          addHitbox(BONEINDEX::spine_2);
        } else {
          removeHitbox(BONEINDEX::spine_2);
        }
      }
      ImGui::GetWindowDrawList()->AddLine(
          ImVec2(StartPos.x + 85, StartPos.y + 165),
          ImVec2(StartPos.x + 190, StartPos.y + 165),
          ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)), 1.8f);  // Penis
      ImGui::SetCursorScreenPos(ImVec2(StartPos.x + 190, StartPos.y + 165));
      if (ImGui::Checkbox(XorStr("###Penis"), &checkbox5)) {
        if (checkbox5) {
          addHitbox(BONEINDEX::pelvis);
        } else {
          removeHitbox(BONEINDEX::pelvis);
        }
      }
      ImGui::SetCursorScreenPos(ImVec2(StartPos.x, StartPos.y + 260));
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::SetCursorPosY(childBegin);
    ImGui::BeginGroup();
    {
      ImGui::BeginChild(
          LangSettings::ChildTriggerbot.c_str(),
          ImVec2(child_size.x,
                 (child_size.y - (style.ItemInnerSpacing.y)) * .55f));
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                            ImVec2(style.FramePadding.x, 0));
        ImGui::Checkbox(LangSettings::SwitchEnableTriggerbot.c_str(),
                        &config::TriggerBot);
        ImGui::HotKey(XorStr(" ##hotkeytrigger"), &config::TriggerBotHotKey);
        ImGui::SliderInt(LangSettings::SliderTriggerDelay.c_str(),
                         &config::TriggerDelay, 15, 1000, XorStr("%d ms"));
        ImGui::SliderInt(LangSettings::SliderShotDuration.c_str(),
                         &config::ShotDuration, 15, 1000, XorStr("%d ms"));
        ImGui::SliderFloat(LangSettings::SliderTriggerMaxRecoil.c_str(),
                           &config::TriggerMaxRecoil, 1.f, 90.f,
                           XorStr("%.1f"));
        ImGui::Checkbox(LangSettings::SwitchTriggerScopeOnly.c_str(),
                        &config::TriggerScopeOnly);
        ImGui::Checkbox(LangSettings::SwitchTriggerIgnoreFlash.c_str(),
                        &config::TriggerIgnoreFlash);
        ImGui::PopStyleVar();
      }
      ImGui::EndChild();

      ImGui::BeginChild(
          LangSettings::ChildRCS.c_str(),
          ImVec2(child_size.x,
                 (child_size.y - (style.ItemInnerSpacing.y)) * .45f));
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                            ImVec2(style.FramePadding.x, 0));
        ImGui::Checkbox(LangSettings::SwitchEnableRCS.c_str(), &config::RCS);
        float RCSScaleValues[2] = {config::RCSScale.x, config::RCSScale.y};
        ImGui::SliderFloat2(LangSettings::SliderRCSScale.c_str(),
                            RCSScaleValues, -.5f, 2.5f, XorStr("%.2f"));
        config::RCSScale = Vector2(RCSScaleValues[0], RCSScaleValues[1]);
        ImGui::SliderInt(LangSettings::SliderRCSStartBullet.c_str(),
                         &config::RCSBullet, 1, 15, XorStr("%d"));
        ImGui::PopStyleVar();
      }
      ImGui::EndChild();

    }
    ImGui::EndGroup();
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem(LangSettings::TabVisuals.c_str())) {
    ImVec2 child_size =
        ImVec2((ImGui::GetColumnWidth() - (style.ItemSpacing.x * 2)) / 3,
               ImGui::GetWindowHeight() - 10.f -
                   (ImGui::GetCursorPosY() + style.ItemInnerSpacing.y * 2));
    static float flOverlayChildSize = 0.f;

    ImGui::BeginChild(LangSettings::ChildESP.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));

      ImGui::Checkbox(LangSettings::SwitchEnableESP.c_str(), &config::ESP);
      ImGui::Checkbox(LangSettings::SwitchPlayerInfo.c_str(), &config::ESPInfo);
      ImGui::Checkbox(LangSettings::SwitchBox.c_str(), &config::ESPBox);
      ImGui::Checkbox(LangSettings::SwitchPlayerName.c_str(), &config::ESPName);
      ImGui::Checkbox(LangSettings::SwitchHealthBar.c_str(), &config::ESPHealth);
      ImGui::PopStyleVar();

      flOverlayChildSize = ImGui::GetCursorPosY() + style.ItemSpacing.y;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild(LangSettings::ChildWorld.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Checkbox(LangSettings::SwitchC4ESP.c_str(), &config::C4ESP);
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild(LangSettings::ChildVisualOthers.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Checkbox(LangSettings::SwitchEnableRadar.c_str(), &config::Radar);
      ImGui::Checkbox(LangSettings::SwitchRadarCrossline.c_str(),
                      &config::RadarCrossLine);
      ImGui::SliderFloat(LangSettings::SliderRadarPointSize.c_str(),
                         &config::RadarPointSize, 0.5f, 5.f, XorStr("%.1f p"));
      ImGui::SliderFloat(LangSettings::SliderRadarProportion.c_str(),
                         &config::RadarProportion, 500.f, 5000.f,
                         XorStr("%.1f u"));
      ImGui::SliderFloat(LangSettings::SliderRadarRange.c_str(),
                         &config::RadarRange, 50.f, 300.f, XorStr("%.1f u"));
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem(LangSettings::TabMiscs.c_str())) {
    ImVec2 child_size =
        ImVec2((ImGui::GetColumnWidth() - (style.ItemSpacing.x * 2)) / 3,
               ImGui::GetWindowHeight() - 10.f -
                   (ImGui::GetCursorPosY() + style.ItemInnerSpacing.y * 2));

    ImGui::BeginChild(LangSettings::ChildMiscGlobals.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Checkbox(LangSettings::SwitchSonar.c_str(), &config::Sonar);
      ImGui::Checkbox(LangSettings::SwitchPitchIndicator.c_str(),
                      &config::PitchIndicator);
      ImGui::Checkbox(LangSettings::SwitchC4Timer.c_str(), &config::C4Timer);
      ImGui::Checkbox(LangSettings::SwitchSpecList.c_str(), &config::SpecList);
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild(LangSettings::ChildMisc.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Checkbox(LangSettings::SwitchEnemyInfo.c_str(),
                      &config::InfoString);
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild(LangSettings::ChildMovement.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Checkbox(LangSettings::SwitchFastStop.c_str(), &config::FastStop);
      ImGui::SliderFloat(LangSettings::SliderFastStopMinVelocity.c_str(),
                         &config::FastStopMinVelocity, 2.5f, 100.f,
                         XorStr("%.1f u/s"));
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem(LangSettings::TabSettings.c_str())) {
    ImVec2 child_size =
        ImVec2((ImGui::GetColumnWidth() - (style.ItemSpacing.x * 2)) / 3,
               ImGui::GetWindowHeight() - 10.f -
                   (ImGui::GetCursorPosY() + style.ItemInnerSpacing.y * 2));
    static char configNameBuffer[64] = "NewConfig";
    static char configAuthorBuffer[64] = "";
    ImGui::BeginChild(XorStr("Info"), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Text(LangSettings::TextLastBuild.c_str(), __DATE__);
      ImGui::Text(LangSettings::TextBuildVersion.c_str(),
                  std::to_string(CS_VERSION).c_str());
      ImGui::Text(LangSettings::TextCurrentGameVersion.c_str(),
                  std::to_string(Vars::GameVersion).c_str());
#ifdef _MSC_VER
#ifndef __clang__
      ImGui::Text((LangSettings::TextCompiler + XorStr(" MSVC %s")).c_str(),
                  std::to_string(_MSC_VER).c_str());
#endif
#endif
#ifdef __GNUC__
#ifndef __clang__
      ImGui::Text((LangSettings::TextCompiler + XorStr(" GCC %s")).c_str(),
                  std::to_string(__GNUC__).c_str());
#endif
#endif
#ifdef __clang__
      ImGui::Text((LangSettings::TextCompiler + XorStr(" Clang %s")).c_str(),
                  std::to_string(__clang_major__).c_str());
#endif
      ImGui::Text(LangSettings::TextLicenceToUser.c_str(),
                  global::userName.c_str());
      ImGui::TextUnformatted(LangSettings::textfortranslator.c_str());
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    MyConfigSaver::UpdateConfigFiles();
    ImGui::BeginChild(LangSettings::ChildConfigList.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(ImGui::GetStyle().FramePadding.x, 0));

      const float itemWidth = 200.0f;
      const float availableWidth = ImGui::GetContentRegionAvail().x;
      const int columns = std::max(1, static_cast<int>(availableWidth / itemWidth));

      ImGui::Columns(columns, NULL, false);

      for (const auto& [filename, modiTimeStr, author] : configFiles) {
        ImGui::PushID(filename.c_str());
        ImGui::BeginChild(
            (XorStr("##") + filename).c_str(), ImVec2(itemWidth - 10, 85), true,
            ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
        {
          ImGui::SetCursorPos(ImVec2(5.f, 3.f));
          std::string configname = filename;
          size_t pos = configname.find_last_of('.');
          if (pos != std::string::npos) {
            configname.erase(pos);
          }
          ImGui::TextUnformatted(configname.c_str());
          ImGui::TextUnformatted(modiTimeStr.c_str());
          ImGui::Text(LangSettings::TextConfigAuthor.c_str(),
                      author.empty() ? XorStr("Akaza Akari") : author.c_str());

          if (selectedConfigFile == filename) {
            if (ImGui::Button(LangSettings::ButtonSave.c_str())) {
              MyConfigSaver::SaveConfig(filename, configAuthorBuffer);
            }
            ImGui::SameLine();
            if (ImGui::Button(LangSettings::ButtonReLoad.c_str())) {
              MyConfigSaver::LoadConfig(filename);
            }
          } else {
            if (ImGui::Button(LangSettings::ButtonLoad.c_str())) {
              MyConfigSaver::LoadConfig(filename);
              selectedConfigFile = filename;
            }
          }
          ImGui::SameLine();
          if (ImGui::Button(LangSettings::ButtonDelete.c_str())) {
            deletePendingFile = filename;
            ImGui::OpenPopup(XorStr("##deleteConfirm"));
          }
          if (ImGui::BeginPopup(XorStr("##deleteConfirm"))) {
            ImGui::Text(LangSettings::TextDeleteConfirm.c_str(),
                        deletePendingFile.c_str());
            if (ImGui::Button(LangSettings::ButtonYes.c_str())) {
              std::string fullPath = config::path + "\\" + deletePendingFile;
              std::remove(fullPath.c_str());
              deletePendingFile.clear();
              ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button(LangSettings::ButtonNo.c_str())) {
              deletePendingFile.clear();
              ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
          }
        }
        ImGui::EndChild();
        if (ImGui::IsItemClicked() && selectedConfigFile != filename) {
          MyConfigSaver::LoadConfig(filename);
          selectedConfigFile = filename;
        }
        bool hovered = ImGui::IsItemHovered();
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImU32 border_color =
            (selectedConfigFile == filename)
                ? ImGui::GetColorU32(ImGuiCol_FrameBgActive)
                : (hovered ? ImGui::GetColorU32(ImGuiCol_FrameBgHovered)
                           : ImGui::GetColorU32(ImGuiCol_FrameBg));
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRect(p_min, p_max, border_color, 0.0f, 0, 1.0f);
        ImGui::PopID();

        ImGui::NextColumn();
      }
      ImGui::Columns(1);

      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild(LangSettings::ChildSettings.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::Checkbox(LangSettings::SwitchTeamCheck.c_str(),
                      &config::TeamCheck);
      ImGui::Checkbox(LangSettings::SwitchBypassCapture.c_str(),
                      &config::BypassCapture);
      ImGui::SliderFloat(LangSettings::SliderDormantTime.c_str(),
                         &config::DormantTime, 0.f, 10.f, XorStr("%.1f s"));
      ImGui::Combo(
          LangSettings::ComboStyle.c_str(), &config::Style,
          XorStr("Lumine\0Aimstar\0ImGui Classic\0ImGui Dark\0ImGui Light\0"));
      MyConfigSaver::RenderLangsFileCombo();
      const float CursorX = 10.f;
      const float ComponentWidth = ImGui::GetColumnWidth() -
                                   ImGui::GetStyle().ItemSpacing.x -
                                   CursorX * 2;

      if (ImGui::Button(LangSettings::ButtonCreateConfig.c_str())) {
        ImGui::OpenPopup(XorStr("##createConfirm"));
      }
      if (ImGui::BeginPopup(XorStr("##createConfirm"))) {
        ImGui::SetNextItemWidth(ComponentWidth);
        ImGui::Text(LangSettings::TextNewConfigName.c_str());
        ImGui::InputText(XorStr("##newConfigName"), configNameBuffer,
                         sizeof(configNameBuffer));

        ImGui::SetNextItemWidth(ComponentWidth);
        ImGui::Text(LangSettings::TextConfigAuthorName.c_str());
        ImGui::InputText(XorStr("##authorName"), configAuthorBuffer,
                         sizeof(configAuthorBuffer));
        std::string configFileName =
            std::string(configNameBuffer) + XorStr(".yaml");
        bool exists = std::any_of(configFiles.begin(), configFiles.end(),
                                  [&](const auto& item) {
                                    return std::get<0>(item) == configFileName;
                                  });

        if (!exists && !configFileName.empty()) {
          if (ImGui::Button(LangSettings::ButtonCreate.c_str())) {
            MyConfigSaver::SaveConfig(configFileName, configAuthorBuffer);
            selectedConfigFile = configFileName;
          }
        } else {
          ImGui::BeginDisabled();
          {
            ImGui::Button(LangSettings::ButtonCreate.c_str());
          }
          ImGui::EndDisabled();
        }
        ImGui::EndPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button(LangSettings::ButtonOpenFolder.c_str())) {
        ShellExecuteA(NULL, XorStr("open"), config::path.c_str(), NULL, NULL,
                      SW_SHOWNORMAL);
      }
      if (ImGui::Button(LangSettings::ButtonExtendFonts.c_str())) {
        ImGui::OpenPopup(XorStr("##extendFonts"));
      }
      if (ImGui::BeginPopup(XorStr("##extendFonts"))) {
        ImGui::Checkbox(XorStr("Greek"), &LangSettings::greek);
        ImGui::Checkbox(XorStr("Viet"), &LangSettings::vietnamese);
        ImGui::Checkbox(XorStr("China&Nippon"), &LangSettings::kanji);
        ImGui::Checkbox(XorStr("Korea"), &LangSettings::korean);
        ImGui::Checkbox(XorStr("Arab"), &LangSettings::arabic);
        ImGui::Checkbox(XorStr("Thai"), &LangSettings::thai);

        if (ImGui::Button(LangSettings::ButtonApply.c_str())) {
          global::fontUpdatePending = true;
        }
        ImGui::EndPopup();
      }
      if (ImGui::Button(LangSettings::ButtonUnhook.c_str())) {
        global::isRunning = false;
      }

      ImGui::Text(XorStr("Menukey [DEL]"));
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem(LangSettings::TabExperimental.c_str())) {
    ImVec2 child_size =
        ImVec2((ImGui::GetColumnWidth() - (style.ItemSpacing.x * 2)) / 3,
               ImGui::GetWindowHeight() - 10.f -
                   (ImGui::GetCursorPosY() + style.ItemInnerSpacing.y * 2));

    ImGui::BeginChild(LangSettings::ChildLoopInterval.c_str(), child_size);
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                          ImVec2(style.FramePadding.x, 0));
      ImGui::SliderInt(LangSettings::SliderRenderFPS.c_str(),
                       &config::RenderFPS, 29, 1000,
                       (config::RenderFPS >= 30)
                           ? XorStr("%d")
                           : LangSettings::TextFPSBased.c_str());
      config::RenderInterval = (config::RenderFPS > 30) ? (1000 / config::RenderFPS) : 0;
      ImGui::SliderInt(LangSettings::SliderGlobalVarsInterval.c_str(),
                       &config::GlobalVarsInterval, 0, 1000,
                       config::GlobalVarsInterval
                           ? XorStr("%d ms")
                           : LangSettings::TextFPSBased.c_str());
      ImGui::SliderInt(LangSettings::SliderEntityInterval.c_str(),
                       &config::EntityInterval, 0, 1000,
                       config::EntityInterval
                           ? XorStr("%d ms")
                           : LangSettings::TextFPSBased.c_str());
      ImGui::SliderInt(LangSettings::SliderParserInterval.c_str(),
                       &config::ParserInterval, 0, 1000,
                       config::ParserInterval
                           ? XorStr("%d ms")
                           : LangSettings::TextFPSBased.c_str());
      ImGui::SliderInt(LangSettings::SliderAimInterval.c_str(),
                       &config::AimInterval, 0, 1000,
                       config::AimInterval
                           ? XorStr("%d ms")
                           : LangSettings::TextFPSBased.c_str());
      ImGui::SliderInt(LangSettings::SliderViewInterval.c_str(),
                       &config::ViewInterval, 0, 1000,
                       config::ViewInterval
                           ? XorStr("%d ms")
                           : LangSettings::TextFPSBased.c_str());
      ImGui::SliderInt(LangSettings::SliderMemoryInterval.c_str(),
                       &config::MemoryInterval, 0, 1000,
                       config::MemoryInterval
                           ? XorStr("%d ms")
                           : LangSettings::TextFPSBased.c_str());
      ImGui::SliderInt(LangSettings::SliderNonMemoryInterval.c_str(),
                       &config::NonMemoryInterval, 0, 1000,
                       config::NonMemoryInterval
                           ? XorStr("%d ms")
                           : LangSettings::TextFPSBased.c_str());
      

      ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    ImGui::EndTabItem();
  }

  ImGui::EndTabBar();
  ImGui::SetCursorPos(ImVec2{style.ItemSpacing.x, 350});
  ImGui::Text(
      XorStr("Samidare for Counter-Strike 2 by Ukia %s"),
      UkiaData::strHWID.substr(UkiaData::strHWID.length() - 16).c_str());

  ImGui::End();
  ImVec2 mousePos = ImGui::GetMousePos();
  float interpolationFactorX = 0.035f;
  float interpolationFactorY = 0.015f;
  ImVec2 center = ImVec2(vecMenuPos.x - 300, vecMenuPos.y + 25);
  float radius = 40;
  ImVec2 factor = ImVec2(interpolationFactorX * (mousePos.x - vecMenuPos.x),
                         interpolationFactorY * (mousePos.y - vecMenuPos.y));
  ImVec2 interpolatedPos{center.x - factor.x, center.y - factor.y};

  // 计算 interpolatedPos 到圆心的距离
  float dx = interpolatedPos.x - center.x;
  float dy = interpolatedPos.y - center.y;
  float distance = sqrt(dx * dx + dy * dy);

  // 如果距离超过半径，则调整 interpolatedPos
  if (distance > radius) {
    float scale = radius / distance;
    factor.x = -dx * scale;
    factor.y = -dy * scale;
    interpolatedPos = ImVec2{center.x - factor.x, center.y - factor.y};
  }

  const ImVec2 vecOverlayPadding = ImVec2(90.f, 30.f);
  ImGui::PopStyleVar();
  ImGui::SetNextWindowPos(interpolatedPos);
  ImGui::Begin(XorStr("moe"), nullptr,
               ImGuiWindowFlags_NoMouseInputs |
                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoFocusOnAppearing |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_::ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground |
                   ImGuiWindowFlags_::ImGuiWindowFlags_NoNavFocus |
                   ImGuiWindowFlags_::ImGuiWindowFlags_NoNav);
  {
    ImGui::SetCursorPos(ImVec2{50, 25});
    ImGui::Image((ImTextureID)global::Shigure, ImVec2{258, 349}, ImVec2{0, 0},
                 ImVec2{1, 1});

    const ImVec2 vecWindPos = ImGui::GetWindowPos();
    const ImVec2 vecWindSize = ImGui::GetWindowSize();

    ImDrawList* pDrawList = ImGui::GetWindowDrawList();
    ImGui::SetCursorPos(ImVec2{20, 40});
    if (config::ESP) {
      ImVec4 vecBox = {
          vecWindPos.x + vecOverlayPadding.x + 50.f, vecWindPos.y + 15,
          vecWindPos.x + vecWindSize.x - vecOverlayPadding.x + 50.f,
          vecWindPos.y + vecWindSize.y - vecOverlayPadding.y};
      vecBox.x -= factor.x * 0.15f;
      vecBox.z -= factor.x * 0.15f;
      vecBox.y -= factor.y * 0.15f;
      vecBox.w -= factor.y * 0.15f;
      if (config::ESPInfo) {
        std::ostringstream playerInfo;
        playerInfo << XorStr("^\n") << XorStr("Health: ") << 100 << XorStr("\n")
                   << XorStr("Dist:") << 114.514f << XorStr("\n");
        DrawStrokeText((vecBox.x + vecBox.z) * 0.5f,
                       vecBox.w, &White,
                       playerInfo.str().c_str());
      }

      if (config::ESPBox) {
        DrawEspBox2D(ImVec2((vecBox.x + vecBox.z) * 0.5f, vecBox.w),
                     ImVec2((vecBox.x + vecBox.z) * 0.5f, vecBox.y),
                     &Red, 1, pDrawList);
      }
      if (config::ESPName)
        DrawNameTag(ImVec2((vecBox.x + vecBox.z) * 0.5f, vecBox.w),
                    ImVec2((vecBox.x + vecBox.z) * 0.5f, vecBox.y),
                    (char*)(XorStr("Not ") + global::userName).c_str());
      float t = ImGui::GetTime();
      int valueHealth = std::abs(std::sin(t)) * 120.f;
      if (config::ESPHealth)
        DrawHealthBar(ImVec2((vecBox.x + vecBox.z) * 0.5f, vecBox.w),
                      ImVec2((vecBox.x + vecBox.z) * 0.5f, vecBox.y),
                      valueHealth, 100, &Green, &Transparent, pDrawList);
    }
    ImGui::SetCursorPos(ImVec2{15 + textoffset, 250 - textoffset} * 1.f +
                        factor * 0.3351f);
    ImGui::TextColored(ImColor(70, 50, 240, 200), XorStr("NOT FOR SELLING!!"));
    ImGui::SetCursorPos(ImVec2{15 + textoffset, 295 - textoffset} * 1.f +
                        factor * 0.1337f);
    ImGui::TextColored(ImColor(70, 50, 240, 200),
                       XorStr("Samidare for Counter-Strike 2"));
    ImGui::SetCursorPos(ImVec2{15 + textoffset, 335 - textoffset} * 1.f +
                        factor * 0.2024f);
    ImGui::TextColored(ImColor(70, 50, 240, 200), XorStr("Build: %s %s"),
                       __DATE__, __TIME__);
    ImGui::SetCursorPos(ImVec2{15 - textoffset, 250 + textoffset} * 1.f +
                        factor * 0.3351f);
    ImGui::TextColored(ImColor(235, 5, 85, 200), XorStr("NOT FOR SELLING!!"));
    ImGui::SetCursorPos(ImVec2{15 - textoffset, 295 + textoffset} * 1.f +
                        factor * 0.1337f);
    ImGui::TextColored(ImColor(235, 5, 85, 200),
                       XorStr("Samidare for Counter-Strike 2"));
    ImGui::SetCursorPos(ImVec2{15 - textoffset, 335 + textoffset} * 1.f +
                        factor * 0.2024f);
    ImGui::TextColored(ImColor(235, 5, 85, 200), XorStr("Build: %s %s"),
                       __DATE__, __TIME__);
    ImGui::SetCursorPos(ImVec2{15, 250} * 1.f + factor * 0.3351f);
    ImGui::TextColored(ImColor(245, 245, 245, 245),
                       XorStr("NOT FOR SELLING!!"));
    ImGui::SetCursorPos(ImVec2{15, 295} * 1.f + factor * 0.1337f);
    ImGui::TextColored(ImColor(245, 245, 245, 245),
                       XorStr("Samidare for Counter-Strike 2"));
    ImGui::SetCursorPos(ImVec2{15, 335} * 1.f + factor * 0.2024f);
    ImGui::TextColored(ImColor(245, 245, 245, 245), XorStr("Build: %s %s"),
                       __DATE__, __TIME__);
  }
  ImGui::End();
}
}  // namespace Menu