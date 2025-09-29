// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://github.com/M3351AN/M3351AN/blob/main/LICENSE
//
// -----------------------------------------------------------------------------
// File: Func.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file is declared functions used in Func.cc.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef FUNC_H_
#define FUNC_H_
#include "pch.h"
#include "../CS2x64.h"
void RenderFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void AimFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void ViewFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void MemoryFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void NonMemoryFunctions() noexcept;
#endif  // FUNC_H_
