// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
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
namespace functions {
void RenderFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void AimFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void ViewFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void MemoryFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void NonMemoryFunctions() noexcept;
}  // namespace functions
#endif  // FUNC_H_
