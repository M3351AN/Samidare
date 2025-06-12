#pragma once
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "..\CS2x64.h"

namespace SpecList {
void RenderWindow(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity);
}