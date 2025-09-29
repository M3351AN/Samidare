// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/1ee25fbd5318d178d15924046fa2060e765b2f66/LICENSE
//
// -----------------------------------------------------------------------------
// File: Aimbot.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file contains declare of aimbot function.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef AIMBOT_H_
#define AIMBOT_H_
#include "pch.h"

#include "../../CS2x64.h"

namespace AimBot
{

    void AimBotRun(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity);
}
#endif  // AIMBOT_H_
