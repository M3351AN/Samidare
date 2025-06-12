#pragma once
#include <atomic>
#include "Overlay.h"

namespace global {
inline std::atomic<bool> isRunning(true);
inline bool isFocused = false;

inline DWORD uiAccessStatus;
inline DWORD processId;

inline HDC hdcBuffer = NULL;
inline HBITMAP hbmBuffer = NULL;

inline RECT gameBounds;
inline HWND hwnd_;

inline Vector2 screenSize;
inline Vector2 screenPos;

inline std::string userName;

inline std::string infos;

inline bool fontUpdatePending = false;

inline IDirect3DTexture9* Shigure = nullptr;
inline IDirect3DTexture9* Zekamashi = nullptr;
}  // namespace global