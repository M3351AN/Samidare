#pragma once
#include <chrono>
#include <thread>

#include "../../CS2x64.h"

namespace AimBot
{

    void AimBotRun(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity);
}