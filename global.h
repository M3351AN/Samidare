// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: global.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file contains globalvars of samidare project.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "pch.h"
#include "Overlay.h"

namespace global {
inline std::atomic<bool> is_running(true);
inline bool is_focused = false;

inline DWORD ui_access_status = 0;
inline DWORD process_id = 0;

// unused
// inline HDC hdc_buffer = NULL;
// inline HBITMAP hbm_buffer = NULL;

// inline RECT game_bounds;
inline HWND game_hwnd;

inline Vector2 screen_size;
inline Vector2 screen_pos;

inline std::string user_name;

inline std::string gamedata_infos;

inline bool is_font_update_pending = false;

inline IDirect3DTexture9* shigure_texture = nullptr;
inline IDirect3DTexture9* zekamashi_texture = nullptr;

inline std::atomic<bool> isRunning() { return is_running.load(); }
inline bool isFocused() { return is_focused; }
inline bool isFontUpdatePending() { return is_font_update_pending; }
}  // namespace global

#endif  // GLOBAL_H_
