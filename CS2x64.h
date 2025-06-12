#pragma once
#include <Windows.h>
#include <Uxtheme.h>
#include <d3d9.h>
#include <dwmapi.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <execution>
#include <iostream>
#include <memory>
#include <mutex>

#include "Functions/ConfigSaver.h"
#include "Game.h"

#ifndef CS_VERSION
#define CS_VERSION 14084
#endif

enum BONEINDEX : DWORD {
  head = 6,
  neck_0 = 5,
  spine_1 = 4,
  spine_2 = 2,
  pelvis = 0,
  arm_upper_L = 8,
  arm_lower_L = 9,
  hand_L = 10,
  arm_upper_R = 13,
  arm_lower_R = 14,
  hand_R = 15,
  leg_upper_L = 22,
  leg_lower_L = 23,
  ankle_L = 24,
  leg_upper_R = 25,
  leg_lower_R = 26,
  ankle_R = 27,
};

struct BoneJointData {
  Vector3 Pos;
  float Scale;
  char pad[0x10];
};

struct BoneJointPos {
  Vector3 Pos;
  Vector2 ScreenPos;
  bool IsVisible = false;
};

class CBone {
 private:
  DWORD64 EntityPawnAddress = 0;

 public:
  std::vector<BoneJointPos> BonePosList;

  bool UpdateAllBoneData(const DWORD64& EntityPawnAddress);
};

namespace BoneJointList {
inline std::list<DWORD> Trunk = {neck_0, spine_2, pelvis};
inline std::list<DWORD> LeftArm = {neck_0, arm_upper_L, arm_lower_L, hand_L};
inline std::list<DWORD> RightArm = {neck_0, arm_upper_R, arm_lower_R, hand_R};
inline std::list<DWORD> LeftLeg = {pelvis, leg_upper_L, leg_lower_L, ankle_L};
inline std::list<DWORD> RightLeg = {pelvis, leg_upper_R, leg_lower_R, ankle_R};
inline std::vector<std::list<DWORD>> List = {Trunk, LeftArm, RightArm, LeftLeg,
                                             RightLeg};
}  // namespace BoneJointList

struct C_UTL_VECTOR {
  DWORD64 Count = 0;
  DWORD64 Data = 0;
};

class PlayerController {
 public:
  std::array<char, 0x8CF> ControllerBuffer{};

  DWORD64 Address = 0;
  int Money = 0;
  int CashSpent = 0;
  int CashSpentTotal = 0;
  int TeamID = 0;
  int Health = 0;
  int AliveStatus = 0;
  int CtrlBot = 0;
  bool Connected = false;
  bool HasHelmet = false;
  bool IsSpec = false;
  INT64 SteamID = 0;
  std::string PlayerName;
  DWORD Pawn = 0;

 public:
  bool GetMoney();
  bool GetTeamID();
  bool GetHealth();
  bool GetIsAlive();
  bool GetIsCtrlBot();
  bool GetConnected();
  bool GetHasHelmet();
  bool GetPlayerSteamID();
  bool GetPlayerName();
  bool GetSpec();
  DWORD64 GetPlayerPawnAddress();

  DWORD64 GetPlayerhPawnAddress();
};

class PlayerPawn {
 public:
  std::array<char, 0x25CF> PawnBuffer{};

  enum class Flags { NONE, IN_AIR = 1 << 0, IN_CROUCH = 1 << 1 };

  DWORD64 Address = 0;
  CBone BoneData;
  Vector2 ViewAngle;
  bool IsDormanted;
  Vector3 Pos;
  Vector2 ScreenPos;
  Vector3 CameraPos;
  Vector3 MaxPos;
  Vector3 MinPos;
  float Height;
  float Width;
  Vector3 Velocity;
  float Speed;
  std::string WeaponName;
  DWORD ShotsFired;
  bool Scoped;
  Vector3 AimPunchAngle;
  C_UTL_VECTOR AimPunchCache;
  int Health;
  int Ammo;
  int MaxAmmo;
  bool IsAuto;
  int Armor;
  int TeamID;
  int Fov;
  bool bSpottedByMask;
  int fFlags;
  float FlashDuration;
  bool isImmunity;

 public:
  bool GetPos();
  bool GetHeight();
  bool GetViewAngle();

  bool GetCameraPos();
  bool GetWeaponName();
  bool GetShotsFired();
  bool GetScoped();
  bool GetAimPunchAngle();
  bool GetHealth();
  bool GetTeamID();
  bool GetFov();
  bool GetSpotted();
  bool GetFFlags();
  bool GetAimPunchCache();
  bool GetAmmo();
  bool GetMaxAmmo();
  bool GetIsAuto();
  bool GetArmor();
  bool GetFlashDuration();
  bool GetIsImmunity();
  bool GetVelocity();
  constexpr bool HasFlag(const Flags Flag) const noexcept {
    return fFlags & (int)Flag;
  }
};

class CEntity {
 public:
  PlayerController Controller;
  PlayerPawn Pawn;

 public:
  // 更新数据
  bool UpdateController(const DWORD64& PlayerControllerAddress);
  bool UpdatePawn(const DWORD64& PlayerPawnAddress);

  // 是否存活
  inline bool IsAlive() {
    return (this->Controller.AliveStatus == 1 ||
            this->Controller.CtrlBot == 1) &&
           this->Pawn.Health > 0;
  }
  inline bool ESPAlive() { return this->Pawn.Health > 0; }
  // 是否在屏幕内
  inline bool IsInScreen() {
    return gGame.View.WorldToScreen(this->Pawn.Pos, this->Pawn.ScreenPos);
  }
  // 获取骨骼数据
  inline CBone GetBone() const {
    if (this->Pawn.Address == 0) return CBone{};
    return this->Pawn.BoneData;
  }

  bool IsEnemy();
  bool IsVisible();
};

class PlantedC4 {
 public:
  std::array<char, 0x14DF> c4Buffer{};

  DWORD64 Address = 0;
  Vector3 Pos;
  bool isPlanted = false;
  bool isDefusing = false;
  bool hasExploded = false;
  float boomTime = 0;
  float defuseTime = 0;
  float boomRemaining = 0;
  float defuseRemaining = 0;
  int bombSite = -1;

 public:
  bool GetIsPlanted();
  bool GetIsDefusing();
  bool GetHasExploded();
  bool GetBoomTime();
  bool GetDefuseTime();
  bool GetBoomRemaining();
  bool GetDefuseRemaining();
  bool GetBombSite();
  bool GetBombPos();

  bool UpdatePlantedC4(const DWORD64& PlantedC4PTRAddress);
};

namespace Vars {
inline std::mutex validEntityMutex;
inline std::mutex VisibleEntityAddrMutex;

inline DWORD64 LocalControllerAddress = 0;
inline DWORD64 LocalPawnAddress = 0;
inline DWORD64 PlantedC4Address = 0;
inline CEntity LocalEntity, ServerEntity;
inline PlantedC4 PlantedBomb;
inline int LocalPlayerControllerIndex = 1;
inline DWORD GameVersion = 0;
inline CGlobalVarsBase Global_Vars = {};
inline std::string MapName = {};
inline float CurTime = 0;
inline int32_t TickCount = 0;
inline float FrameTime = 0;
inline std::vector<std::pair<CEntity, DWORD64>> ValidEntity = {};
inline std::vector<DWORD64> VisibleEntityAddr = {};
inline bool FreeFire = false;
inline bool IsInGame = false;
inline float Sensitivity = 0.f;
inline bool IsMapFileExist = false;

inline int RenderInterval = 0;
inline int GlobalVarsInterval = 15;
inline int EntityInterval = 1;
inline int ParserInterval = 100;
inline int AimInterval = 1;
inline int ViewInterval = 1;
inline int MemoryInterval = 15;
inline int NonMemoryInterval = 1;

inline bool IsAimbotting = false;

void ParserRun() noexcept;
void UpdateDataSlow();
void UpdateData();
void UpdateIntervals();
}  // namespace Vars

namespace DirectX9Interface {
inline IDirect3D9Ex* Direct3D9 = NULL;
inline IDirect3DDevice9Ex* pDevice = NULL;
inline D3DPRESENT_PARAMETERS pParams = {NULL};
inline MARGINS Margin = {-1};
inline MSG Message = {NULL};
}  // namespace DirectX9Interface

namespace OverlayWindow {
inline WNDCLASSEXA WindowClass;
inline HWND Hwnd;
inline LPCSTR Name;
}  // namespace OverlayWindow