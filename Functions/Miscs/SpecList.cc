// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: SpecList.cc
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-30
//
// Description:
//   This file contains spectator list functions.
//
// -----------------------------------------------------------------------------
#include "pch.h"
#include "SpecList.h"

#include "../../UkiaStuff.h"
#include "../config.h"

namespace spectatorlist {

  inline uintptr_t get_pcs_player_pawn(uintptr_t entity_list, uintptr_t pawn) {
  uintptr_t l_list_entry2;
   Ukia::ProcessMgr.ReadMemory<uintptr_t>(entity_list +
                                         0x8 * ((pawn & 0x7FFF) >> 9) + 16,l_list_entry2);
    if (!l_list_entry2) return 0;
   uintptr_t RETURN;
    Ukia::ProcessMgr.ReadMemory<uintptr_t>(l_list_entry2 + 120 * (pawn & 0x1FF),
                                           RETURN);
    return RETURN;
  }

void RenderWindow(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) {
  if (!config::SpecList) return;
  ImGui::SetNextWindowSize(ImVec2(200, -1));
  ImVec4 default_bg_color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
  default_bg_color.w = 0.5f;
  ImGui::PushStyleColor(ImGuiCol_WindowBg, default_bg_color);
  ImGui::Begin(XorStr("SPECTATORS"), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
                    ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_::ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_::ImGuiWindowFlags_NoNavFocus |
                   ImGuiWindowFlags_::ImGuiWindowFlags_NoNav);
  { 
    ImGui::SetCursorPosY(22);
    for (int index = 0; index < ValidEntity.size(); index++) {
      CEntity Entity = ValidEntity[index].first;
      // DWORD64 EntityAddress = ValidEntity[index].second;

      if (Entity.Controller.IsSpec) {
        const char* Name = (Entity.Controller.PlayerName).c_str();
        ImGui::Text("%s", Name);
      }
      }
  }
  ImGui::End();
  ImGui::PopStyleColor();
}
}  // namespace SpecList
