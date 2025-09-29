// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: TriggerBot.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-30
//
// Description:
//   This file contains declare of trigger bot functions.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef TRIGGERBOT_H_
#define TRIGGERBOT_H_
#include "pch.h"

#include "../../CS2x64.h"

namespace triggerbot
{
	inline unsigned int HotKey = VK_XBUTTON2;
	inline std::chrono::time_point<std::chrono::system_clock> timepoint = std::chrono::system_clock::now();
	inline std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
	inline bool recorded = false;
    inline bool isAim = false;
	// Triggerbot
    void TriggerBotRun(const CEntity& LocalEntity);
}
#endif
