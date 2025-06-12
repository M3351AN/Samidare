#pragma once
#include <chrono>
#include <iostream>
#include <utility>
#include <sstream>
#include <ctime>
#include <string>
#include "..\CS2x64.h"

namespace BombTimer
{
std::pair<int, int> get_bomb_calculations_by_map(const std::string& map);
float armor_modifier(float damage, int armor);
int calculate_bomb_damage(Vector3 player, Vector3 bomb, int armor);
void RenderWindow(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity);
}