// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: BombTimer.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-30
//
// Description:
//   This file contains declare of bomb timer function.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef BOMBTIMER_H_
#define BOMBTIMER_H_
#include "pch.h"
#include "../../CS2x64.h"

namespace bombtimer
{
std::pair<int, int> get_bomb_calculations_by_map(const std::string& map);
float armor_modifier(float damage, int armor);
int calculate_bomb_damage(Vector3 player, Vector3 bomb, int armor);
void RenderWindow(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity);
}
#endif
