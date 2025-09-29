// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: RCS.cc
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file contains recoil control system function.
//
// -----------------------------------------------------------------------------
#include "pch.h"
#include "RCS.h"

#include "../../Driver.h"
namespace RCS {

void RCSRun(CEntity& Local) noexcept {
  if (!config::RCS) return;
  static Vector3 OldPunch;
  if (Local.Pawn.ShotsFired > static_cast<unsigned int>(config::RCSBullet) &&
      !gamevars::IsAimbotting) {
    Vector3 delta = (OldPunch - (Local.Pawn.AimPunchAngle));

    int MouseX = static_cast<int>(std::round(
        (delta.y * config::RCSScale.x / gamevars::Sensitivity) / -0.022f));
    int MouseY = static_cast<int>(std::round(
        (delta.x * config::RCSScale.y / gamevars::Sensitivity) / 0.022f));

    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000))
#ifndef USERMODE
      driver.mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
#else
      my_mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
#endif
    OldPunch = Local.Pawn.AimPunchAngle;
  } else {
    OldPunch = Local.Pawn.AimPunchAngle;
  }
}
}  // namespace RCS
