// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/1ee25fbd5318d178d15924046fa2060e765b2f66/LICENSE
//
// -----------------------------------------------------------------------------
// File: BombTimer.cc
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file contains bomb timer function.
//
// -----------------------------------------------------------------------------
#include "pch.h"
#include "BombTimer.h"

#include "../../UkiaStuff.h"
#include "../config.h"

namespace BombTimer {
std::pair<int, int> get_bomb_calculations_by_map(const std::string& map) {
  // get these with map_showbombradius
  if (map == "de_dust2") {
    return {500, 1750};
  } else if (map == "de_ancient") {
    return {650, 2275};
  } else if (map == "de_anubis") {
    return {450, 1575};
  } else if (map == "de_inferno") {
    return {620, 2170};
  } else if (map == "de_mirage") {
    return {650, 2275};
  } else if (map == "de_nuke") {
    return {650, 2275};
  } else if (map == "de_overpass") {
    return {650, 2275};
  } else if (map == "de_vertigo") {
    return {500, 1750};
  } else if (map == "de_train") {
    return {500, 1750};
  } else if (map == "de_assembly") {
    return {500, 1750};
  } else if (map == "de_memento") {
    return {500, 1750};
  } else if (map == "de_thera") {
    return {500, 1750};
  } else if (map == "de_mills") {
    return {500, 1750};
  } else if (map == "de_dogtown") {
    return {650, 2275};
  } else if (map == "de_brewery") {
    return {500, 1750};
  } else if (map == "de_jura") {
    return {500, 1750};
  } else if (map == "de_grail") {
    return {500, 1750};
  } else {
    return {500, 1750};
  }
}
float armor_modifier(float damage, int armor) {
  if (armor > 0) {
    const float armor_ratio = 0.5f;
    const float armor_bonus = 0.5f;
    float armor_ratio_multiply = damage * armor_ratio;
    float actual = (damage - armor_ratio_multiply) * armor_bonus;

    if (actual > static_cast<float>(armor)) {
      actual = static_cast<float>(armor) * (1.f / armor_bonus);
      armor_ratio_multiply = damage - actual;
    }

    damage = armor_ratio_multiply;
  }
  return damage;
}

int calculate_bomb_damage(Vector3 player, Vector3 bomb, int armor) {
  const std::pair<int, int> bomb_calculations =
      get_bomb_calculations_by_map(gamevars::MapName);
  const int bomb_damage = bomb_calculations.first;
  const int bomb_radius = bomb_calculations.second;

  const double c = bomb_radius / 3;
  const float damage = static_cast<float>(bomb_damage *
                       std::exp(-std::pow(sqrt(pow(player.x - bomb.x, 2) +
                                            pow(player.y - bomb.y, 2) +
                                            pow(player.z - bomb.z, 2)),
                                       2) / (2 * std::pow(c, 2))));
  const float damage_armor = armor_modifier(damage, armor);

  return static_cast<int>(std::ceil(
      damage_armor));  // if wanna more accurte, use std::round, ceil is to
                       // estim max damage to avoid unexpected death
}

void RenderWindow(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) {
  if (!config::C4Timer) return;
  int damage = 0;

  if (gamevars::PlantedBomb.isPlanted && gamevars::PlantedBomb.boomRemaining > 0) {
    damage =
        calculate_bomb_damage(gamevars::LocalEntity.Pawn.Pos, gamevars::PlantedBomb.Pos,
                              gamevars::LocalEntity.Pawn.Armor);
  } else {
    damage = 0;
  }

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
  ImVec4 default_bg_color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
  default_bg_color.w = 0.5f;
  ImGui::PushStyleColor(ImGuiCol_WindowBg, default_bg_color);
  ImGui::SetNextWindowSizeConstraints(ImVec2(224, 95), ImVec2(500, 95));
  ImGui::Begin(XorStr("Bomb Timer"), nullptr, flags);

  float windowWidth = ImGui::GetWindowSize().x;
  float barLength = gamevars::PlantedBomb.boomRemaining <= 0.0f ? 0.0f
                    : gamevars::PlantedBomb.boomRemaining >= 40
                        ? 1.0f
                        : (gamevars::PlantedBomb.boomRemaining / 40.0f);
  if (gamevars::PlantedBomb.isPlanted && gamevars::PlantedBomb.boomRemaining > 0 &&
      !gamevars::PlantedBomb.hasExploded) {
    std::string text = LangSettings::TextBombOn;
    char buf[128];
    snprintf(buf, sizeof(buf), text.c_str(),
             (!gamevars::PlantedBomb.bombSite ? XorStr("A") : XorStr("B")),
             gamevars::PlantedBomb.boomRemaining);
    float textWidth = ImGui::CalcTextSize(buf).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(XorStr("%s"), buf);

    float barWidth = windowWidth - 20;
    ImGui::SetCursorPosX((windowWidth - barWidth) * 0.5f);
    ImGui::ProgressBar(barLength, ImVec2(barWidth, 15), "");

    text = LangSettings::TextBombEstimDamage;
    snprintf(buf, sizeof(buf), text.c_str(), damage);
    textWidth = ImGui::CalcTextSize(buf).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(XorStr("%s"), buf);

    if (gamevars::PlantedBomb.isDefusing) {
      std::string defuseText;
      snprintf(buf, sizeof(buf), LangSettings::TextDefuseCount.c_str(),
               (gamevars::PlantedBomb.defuseTime < gamevars::PlantedBomb.boomTime)
                   ? LangSettings::TextCanDefuse.c_str()
                   : LangSettings::TextCanNotDefuse.c_str(),
               gamevars::PlantedBomb.defuseRemaining);
      float defuseTextWidth = ImGui::CalcTextSize(buf).x;
      ImGui::SetCursorPosX((windowWidth - defuseTextWidth) * 0.5f);
      ImVec4 color = (gamevars::PlantedBomb.defuseTime < gamevars::PlantedBomb.boomTime)
                         ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f)
                         : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
      ImGui::PushStyleColor(ImGuiCol_Text, color);
      ImGui::Text("%s", buf);
      ImGui::PopStyleColor();
    } else {
      text = LangSettings::TextNotDefusing;
      snprintf(buf, sizeof(buf), text.c_str(), damage);
      textWidth = ImGui::CalcTextSize(buf).x;
      ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
      ImGui::Text(XorStr("%s"), buf);
    }
  } else {
    std::string text = LangSettings::TextBombNotPlanted;
    float textWidth = ImGui::CalcTextSize(text.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(XorStr("%s"), text.c_str());

    float barWidth = windowWidth - 20;
    ImGui::SetCursorPosX((windowWidth - barWidth) * 0.5f);
    ImGui::ProgressBar(0.0f, ImVec2(barWidth, 15), "");
  }

  ImGui::PopStyleColor();
  ImGui::End();
}
}  // namespace BombTimer
