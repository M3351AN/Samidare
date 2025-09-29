// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: SpecList.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-30
//
// Description:
//   This file contains declare of spectator list functions.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef SPECLIST_H_
#define SPECLIST_H_
#include "pch.h"

#include "../../CS2x64.h"

namespace spectatorlist {
void RenderWindow(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity);
}
#endif
