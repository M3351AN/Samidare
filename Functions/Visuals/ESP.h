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
// Date: 2025-09-30
//
// Description:
//   This file contains declare of ESP functions.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef ESP_H_
#define ESP_H_
#include "pch.h"
#include "../../CS2x64.h"

namespace esp {

void ESPRun(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
}  // namespace ESP
#endif
