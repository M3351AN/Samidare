// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: Game.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-29
//
// Description:
//   This file contains functions and classes for game interaction and
//   world-to-screen.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef GAME_H_
#define GAME_H_
#include <optional>
#include "global.h"
#include "Offsets.h"
#include "Overlay.h"
class CView {
 public:
  float Matrix[4][4]{};

  bool WorldToScreen(const Vector3& Pos, Vector2& ToPos) {
    float SightX = global::screen_size.x * .5f;
    float SightY = global::screen_size.y * .5f;

    float m30 = Matrix[3][0], m31 = Matrix[3][1], m32 = Matrix[3][2],
          m33 = Matrix[3][3];
    float m00 = Matrix[0][0], m01 = Matrix[0][1], m02 = Matrix[0][2],
          m03 = Matrix[0][3];
    float m10 = Matrix[1][0], m11 = Matrix[1][1], m12 = Matrix[1][2],
          m13 = Matrix[1][3];

    float View = m30 * Pos.x + m31 * Pos.y + m32 * Pos.z + m33;

    if (View <= 0.01f) return false;

    float invView = 1.0f / View;
    ToPos.x = SightX + (m00 * Pos.x + m01 * Pos.y + m02 * Pos.z + m03) *
                           invView * SightX;
    ToPos.y = SightY - (m10 * Pos.x + m11 * Pos.y + m12 * Pos.z + m13) *
                           invView * SightY;

    return true;
  }

  Vector2 GetScreenCenterVec2() {
    Vector2 Pos = global::screen_size * .5f;
    return Pos;
  }
};
class CGame
{
private:
	struct
	{
		DWORD64 ForceJump;
		DWORD64 ForceCrouch;
		DWORD64 ForceForward;
		DWORD64 ForceLeft;
		DWORD64 ForceRight;

		DWORD64 ClientDLL;
        DWORD64 EngineDLL;
        DWORD64 ServerDLL;
        DWORD64 Tier0DLL;
		DWORD64 EntityList;
		DWORD64 Matrix;
		DWORD64 ViewAngle;
		DWORD64 EntityListEntry;
		DWORD64 LocalController;
		DWORD64 LocalPawn;
		DWORD64 ServerPawn;
        DWORD64 PlantedC4PTR;
		DWORD64 GlobalVars;
		DWORD64 CSGOInput;
        DWORD64 BuildNumber;
	}Address;

public:
	CView View;

public:	

	bool InitAddress();

	DWORD64 GetClientDLLAddress();
    DWORD64 GetEngineDLLAddress();
	DWORD64 GetServerDLLAddress();
    DWORD64 GetTier0DLLAddress();

	DWORD64 GetEntityListAddress();

	DWORD64 GetMatrixAddress();

	DWORD64 GetViewAngleAddress();

	DWORD64 GetEntityListEntry();

	DWORD64 GetLocalControllerAddress();

	DWORD64 GetLocalPawnAddress();

	DWORD64 GetServerPawnAddress();

    DWORD64 GetPlantedC4PTRAddress();

	DWORD64 GetGlobalVarsAddress();

	DWORD64 GetCSGOInputAddress();

	DWORD64 GetBuildNumberAddress();
        
	bool UpdateEntityListEntry();

    bool GetViewAngles(Vector3& Angle);
    bool SetViewAngles(Vector3 Angle);

	bool SetForceJump(int Value);
	bool GetForceJump(int& Value);
	bool SetForceCrouch(int Value);
	bool GetForceCrouch(int& Value);
	bool SetForceMove(int MovingType, int Value);
	bool GetForceMove(int MovingType, int& Value);
};

inline CGame gGame;
#endif  // GAME_H_
