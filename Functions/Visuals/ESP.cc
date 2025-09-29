// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: ESP.cc
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file contains ESP functions.
//
// -----------------------------------------------------------------------------
#include "pch.h"
#include "ESP.h"

namespace esp {

static std::unordered_map<uintptr_t, float> g_dormantStartTimes;

void ESPRun(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!gamevars::IsInGame) return;
  auto now = std::chrono::steady_clock::now();
  float currentTime =
      std::chrono::duration<float>(now.time_since_epoch()).count();
  if (config::ESP) {
    for (int index = 0; index < ValidEntity.size(); index++) {
      CEntity Entity = ValidEntity[index].first;
      DWORD64 EntityAddress = ValidEntity[index].second;
      if (!Entity.IsEnemy()) continue;

      if (Entity.Pawn.IsDormanted) {
        if (g_dormantStartTimes.find(EntityAddress) ==
            g_dormantStartTimes.end()) {
          g_dormantStartTimes[EntityAddress] = currentTime;
        }

        if (currentTime - g_dormantStartTimes[EntityAddress] >
            config::DormantTime) {
          continue;
        }
      } else {
        g_dormantStartTimes.erase(EntityAddress);
      }

      Vector3 vecPos = Entity.Pawn.Pos;

      float dist = vecPos.DistanceTo(gamevars::LocalEntity.Pawn.Pos);
      Vector2 screen_pos = Entity.Pawn.ScreenPos;

      if (Entity.ESPAlive() && Entity.IsInScreen()) {
        Vector3 head_pos = {vecPos.x, vecPos.y, vecPos.z + Entity.Pawn.Height};
        Vector2 head_screen_pos;
        gGame.View.WorldToScreen(head_pos, head_screen_pos);

        if (config::ESPInfo) {
          std::ostringstream playerInfo;
          playerInfo << XorStr("^\n") << XorStr("Health: ")
                     << Entity.Pawn.Health << XorStr("\n") << XorStr("Dist:")
                     << dist << XorStr("\n");
          if (Entity.Pawn.IsDormanted || Entity.Pawn.isImmunity)
            DrawStrokeText(screen_pos.x, screen_pos.y, &Grey,
                           playerInfo.str().c_str());
          else
            DrawStrokeText(screen_pos.x, screen_pos.y, &White,
                           playerInfo.str().c_str());
        }

        if (config::ESPBox) {
          if (Entity.Pawn.IsDormanted || Entity.Pawn.isImmunity)
            DrawEspBox2D(screen_pos, head_screen_pos, &Grey, 1);
          else if (Entity.IsVisible())
            DrawEspBox2D(screen_pos, head_screen_pos, &Red, 1);
          else
            DrawEspBox2D(screen_pos, head_screen_pos, &Green, 1);
        }
        if (config::ESPName)
          DrawNameTag(screen_pos, head_screen_pos,
                      (char*)Entity.Controller.PlayerName.c_str());
        if (config::ESPHealth)
          DrawHealthBar(screen_pos, head_screen_pos, Entity.Pawn.Health);
      }
    }
  }
  if (config::C4ESP && gamevars::PlantedBomb.isPlanted &&
      gamevars::PlantedBomb.boomRemaining > 0) 
      {
    Vector2 screen_pos;
    gGame.View.WorldToScreen(gamevars::PlantedBomb.Pos, screen_pos);
    Vector2 head_screen_pos;
    gGame.View.WorldToScreen(
        Vector3(gamevars::PlantedBomb.Pos.x, gamevars::PlantedBomb.Pos.y,
                              gamevars::PlantedBomb.Pos.z+10.f),
                             head_screen_pos);
    DrawEspBox2D(screen_pos, head_screen_pos, &Green, 1);
    DrawNameTag(screen_pos, head_screen_pos, XorStr("C4"));
  }
}
}  // namespace ESP
