#pragma once
#include <chrono>
#include <thread>

#include "../../CS2x64.h"

namespace TriggerBot
{
	inline unsigned int HotKey = VK_XBUTTON2;
	inline std::chrono::time_point<std::chrono::system_clock> timepoint = std::chrono::system_clock::now();
	inline std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
	inline bool recorded = false;
    inline bool isAim = false;
	// Triggerbot
    void TriggerBotRun(const CEntity& LocalEntity);
}