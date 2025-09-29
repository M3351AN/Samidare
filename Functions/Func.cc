// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://github.com/M3351AN/M3351AN/blob/main/LICENSE
//
// -----------------------------------------------------------------------------
// File: Func.cc
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file is trunk of all functions in the Samidare project.
//
// -----------------------------------------------------------------------------
#include "pch.h"
#include "Func.h"

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
  if (!global::isFocused()) return;
  std::lock_guard<std::mutex> lock(gamevars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  ESP::ESPRun(EntityList);
  Radar::RadarRun(EntityList);
  Misc::PitchIndicator(gamevars::LocalEntity);
  BombTimer::RenderWindow(EntityList);
  SpecList::RenderWindow(EntityList);
  Menu::DrawMenu();
  DrawNewText(10, 10, &White, XorStr("Samidare for Counter-Strike 2"));
  DrawNewText(10, 100, &White, global::gamedata_infos.c_str());
  DrawNewText(
      10, ImGui::GetIO().DisplaySize.y - 20, &White,
      UkiaData::strHWID.substr(UkiaData::strHWID.length() - 16).c_str());
}
void AimFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!global::isFocused() || config::ShowMenu) return;
  if (!gamevars::IsInGame) return;
  if (!gamevars::LocalEntity.Controller.Connected) return;
  std::lock_guard<std::mutex> lock(gamevars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  AimBot::AimBotRun(EntityList);
  TriggerBot::TriggerBotRun(gamevars::LocalEntity);
  RCS::RCSRun(gamevars::LocalEntity);
}
  void ViewFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!global::isFocused()) return;
  if (!gamevars::IsInGame) return;
  std::lock_guard<std::mutex> lock(gamevars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  Misc::SonarRun(EntityList);
  Misc::FastStop(gamevars::LocalEntity);
  }
void MemoryFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!global::isFocused()) return;
  if (!gamevars::IsInGame) return;
  std::lock_guard<std::mutex> lock(gamevars::validEntityMutex);
  std::vector<std::pair<CEntity, DWORD64>> EntityList = ValidEntity;
  Misc::FoundEnemy(EntityList);
}
void NonMemoryFunctions() noexcept {
  if (!global::isFocused()) return;
  if (!gamevars::IsInGame) return;
  Sonar::SoundThread();
}
