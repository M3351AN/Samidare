#include "Func.h"

#include <sstream>

#include "../Overlay.h"
#include "Aimbot/Aimbot.h"
#include "Aimbot/TriggerBot.h"
#include "Aimbot/RCS.h"
#include "Miscs/Miscs.h"
#include "Visuals/ESP.h"
#include "Visuals/Radar.h"
#include "menu.h"
#include "Miscs/BombTimer.h"
#include "Miscs/SpecList.h"
#include "../UkiaStuff.h"
void RenderFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!global::isFocused) return;
  std::lock_guard<std::mutex> lock(Vars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  ESP::ESPRun(EntityList);
  Radar::RadarRun(EntityList);
  Misc::PitchIndicator(Vars::LocalEntity);
  BombTimer::RenderWindow(EntityList);
  SpecList::RenderWindow(EntityList);
  Menu::DrawMenu();
  DrawNewText(10, 10, &White, XorStr("Samidare for Counter-Strike 2"));
  DrawNewText(10, 100, &White, global::infos.c_str());
  DrawNewText(
      10, ImGui::GetIO().DisplaySize.y - 20, &White,
      UkiaData::strHWID.substr(UkiaData::strHWID.length() - 16).c_str());
}
void AimFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!global::isFocused || config::ShowMenu) return;
  if (!Vars::IsInGame) return;
  if (!Vars::LocalEntity.Controller.Connected) return;
  std::lock_guard<std::mutex> lock(Vars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  AimBot::AimBotRun(EntityList);
  TriggerBot::TriggerBotRun(Vars::LocalEntity);
  RCS::RCSRun(Vars::LocalEntity);
}
  void ViewFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!global::isFocused) return;
  if (!Vars::IsInGame) return;
  std::lock_guard<std::mutex> lock(Vars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  Misc::SonarRun(EntityList);
  Misc::FastStop(Vars::LocalEntity);
  }
void MemoryFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!global::isFocused) return;
  if (!Vars::IsInGame) return;
  std::lock_guard<std::mutex> lock(Vars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  Misc::FoundEnemy(EntityList);
}
void NonMemoryFunctions() noexcept {
  if (!global::isFocused) return;
  if (!Vars::IsInGame) return;
  Sonar::SoundThread();
}