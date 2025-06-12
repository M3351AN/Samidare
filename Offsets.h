#pragma once
#include <windows.h>
#include <cstdint>
/*
We using a2x header for quick PoC while develop & to figure out type of data
In the end, we use our own offset system by reading offset.yaml for release
version.
*/
class CGlobalVarsBase {
 public:
  float m_realtime;            // 0x0000
  int32_t m_framecount;        // 0x0004
  float m_frametime;           // 0x0008
  float m_abs_frametime;       // 0x000C
  int32_t m_maxclients;        // 0x0010
  char pad_0014[28];           // 0x0014
  float m_frametime2;          // 0x0030
  float m_curtime;             // 0x0034
  float m_curtime2;            // 0x0038
  char pad_003C[20];           // 0x003C
  int32_t m_tickcount;         // 0x0050
  char pad_0054[292];          // 0x0054
  uintptr_t m_current_map;      // 0x0178
  uintptr_t m_current_mapname;  // 0x0180
};

namespace Offset {

extern uintptr_t ForceJump;
extern uintptr_t ForceCrouch;
extern uintptr_t ForceForward;
extern uintptr_t ForceLeft;
extern uintptr_t ForceRight;

extern uintptr_t EntityList;
extern uintptr_t Matrix;
extern uintptr_t ViewAngle;
extern uintptr_t LocalPlayerController;
extern uintptr_t LocalPlayerPawn;
extern uintptr_t GlobalVars;
extern uintptr_t CSGOInput;
extern uintptr_t InventoryServices;
extern uintptr_t PlantedC4;
extern uintptr_t InputSystem;
extern uintptr_t dwSensitivity;
extern uintptr_t Sensitivity;
extern uintptr_t BuildNumber;
extern uintptr_t dwNetworkGameClient_signOnState;
extern uintptr_t VEngineCvar007;

              // yaml(need
         // manual dump tho)
// entity stuff
struct C_BaseEntityT {
  DWORD m_flGravityScale;
  DWORD m_iMaxHealth;      // C_BaseEntity::m_iMaxHealth
  DWORD m_iHealth;         // C_BaseEntity::m_iHealth
  DWORD m_pGameSceneNode;  // C_BaseEntity::m_pGameSceneNode
  DWORD m_pCollision;
  DWORD m_vecAbsVelocity;
  DWORD m_fFlags;
  DWORD m_iTeamNum;
} ;
extern C_BaseEntityT C_BaseEntity;
struct C_BaseModelEntityT {
  DWORD GlowFunction;
  DWORD GlowColorOverride;
};
extern C_BaseModelEntityT C_BaseModelEntity;
// gamescenenode stuff
struct CGameSceneNodeT {
  DWORD m_vecOrigin;
  DWORD m_bDormant;
};
extern CGameSceneNodeT CGameSceneNode;

struct CCollisionPropertyT {
  DWORD m_vecMins;
  DWORD m_vecMaxs;
};
extern CCollisionPropertyT CCollisionProperty;
// contoller stuff
struct CBasePlayerControllerT {
  DWORD m_hPawn;
  DWORD m_iszPlayerName;
  DWORD m_iDesiredFOV;
  DWORD m_steamID;
};
extern CBasePlayerControllerT CBasePlayerController;

struct CCSPlayerControllerT {
  DWORD m_bPawnIsAlive;
  DWORD m_bControllingBot;
  DWORD m_bEverPlayedOnTeam;
  DWORD m_hPlayerPawn;
  DWORD m_sSanitizedPlayerName;
  DWORD m_iPawnArmor;
  DWORD m_bPawnHasDefuser;
  DWORD m_bPawnHasHelmet;
  DWORD m_iPing;
  DWORD m_pInGameMoneyServices;
};
extern CCSPlayerControllerT CCSPlayerController;
    // pawn stuff
struct C_BasePlayerPawnT {
  DWORD m_pMovementServices;  // CPlayer_MovementServices*
  DWORD m_pWeaponServices;    // CPlayer_WeaponServices*
  DWORD m_pCameraServices;    // CPlayer_CameraServices*
  DWORD m_pObserverServices;
  DWORD m_vOldOrigin;  // C_BasePlayerPawn::m_vOldOrigin
  DWORD m_hController;
};
extern C_BasePlayerPawnT C_BasePlayerPawn;

struct C_CSPlayerPawnBaseT {
  DWORD m_pViewModelServices;  // CPlayer_ViewModelServices*
  DWORD m_pClippingWeapon;     // C_CSWeaponBase*
  DWORD m_angEyeAngles;
  DWORD m_bGunGameImmunity;
  DWORD m_vecLastClipCameraPos;
  DWORD m_flFlashMaxAlpha;
  DWORD m_flFlashDuration;
  DWORD m_iIDEntIndex;
};
extern C_CSPlayerPawnBaseT C_CSPlayerPawnBase;

struct C_CSPlayerPawnT {
  DWORD m_pBulletServices;  // CCSPlayer_BulletServices*
  DWORD m_bIsScoped;
  DWORD m_bIsDefusing;
  DWORD m_ArmorValue;  // C_CSPlayerPawn::m_ArmorValue
  DWORD m_iShotsFired;
  DWORD m_aimPunchAngle;  // C_CSPlayerPawn::m_aimPunchAngle
  DWORD m_aimPunchCache;
  DWORD m_bIsBuyMenuOpen;
  DWORD m_bWaitForNoAttack;
  DWORD m_bSpottedByMask;  // C_CSPlayerPawn::entitySpottedState +
                           // EntitySpottedState_t::bSpottedByMask
};
extern C_CSPlayerPawnT C_CSPlayerPawn;
// C4 stuff
struct C_PlantedC4T {
  DWORD m_flC4Blow;
  DWORD m_bC4Activated;
  DWORD m_bBeingDefused;
  DWORD m_bHasExploded;
  DWORD m_flDefuseCountDown;
  DWORD m_hBombDefuser;
  DWORD m_nBombSite;
};
extern C_PlantedC4T C_PlantedC4;
// services stuff
struct CPlayer_ObserverServicesT {
  DWORD m_hObserverTarget;
  DWORD m_iObserverMode;
};
extern CPlayer_ObserverServicesT CPlayer_ObserverServices;
struct CCSPlayer_ViewModelServicesT {
  DWORD m_hViewModel;  // CCSPlayer_ViewModelServices::m_hViewModel
};
extern CCSPlayer_ViewModelServicesT CCSPlayer_ViewModelServices;
struct CPlayer_MovementServices_HumanoidT {
  DWORD m_nCrouchState;
};
extern CPlayer_MovementServices_HumanoidT CPlayer_MovementServices_Humanoid;
struct CCSPlayerBase_CameraServicesT {
  DWORD m_iFOVStart;
};
extern CCSPlayerBase_CameraServicesT CCSPlayerBase_CameraServices;
struct CCSPlayer_BulletServicesT {
  DWORD m_totalHitsOnServer;
};
extern CCSPlayer_BulletServicesT CCSPlayer_BulletServices;
struct CCSPlayerController_InGameMoneyServicesT {
  DWORD m_iAccount;
  DWORD m_iTotalCashSpent;
  DWORD m_iCashSpentThisRound;
};
extern CCSPlayerController_InGameMoneyServicesT
    CCSPlayerController_InGameMoneyServices;

// random misc stuff
struct EconEntityT {
  DWORD AttributeManager;  // C_AttributeContainer
  DWORD FallbackPaintKit;
  DWORD FallbackSeed;
  DWORD FallbackWear;
  DWORD FallbackStatTrak;
  DWORD szCustomName;
  DWORD EntityQuality;  // EconItemView::m_iEntityQuality
  DWORD ItemIDHigh;     // EconItemView::m_iItemIDHigh
};
extern EconEntityT EconEntity;

struct WeaponBaseDataT {
  DWORD WeaponDataPTR;
  DWORD szName;
  DWORD Clip1;    // C_BasePlayerWeapon::m_iClip1
  DWORD MaxClip;  // CBasePlayerWeaponVData::m_iMaxClip1
  DWORD CycleTime;
  DWORD Penetration;
  DWORD WeaponType;
  DWORD Inaccuracy;  // CCSWeaponBaseVData::m_flInaccuracyMove
  DWORD inReload;
  DWORD m_nNumBullets;
  DWORD WeaponSize;
  DWORD ActiveWeapon;
  DWORD Item;  // C_AttributeContainer::m_Item
  DWORD ItemDefinitionIndex;
  DWORD m_MeshGroupMask;  // CModelState::m_MeshGroupMask
  DWORD m_bIsFullAuto;
};
extern WeaponBaseDataT WeaponBaseData;

struct SmokeGrenadeProjectileT  // C_BaseCSGrenadeProjectile
{
  DWORD nSmokeEffectTickBegin;     // int32_t
  DWORD bDidSmokeEffect;           // bool
  DWORD nRandomSeed;               // int32_t
  DWORD vSmokeColor;               // Vector
  DWORD vSmokeDetonationPos;       // Vector
  DWORD VoxelFrameData;            // CUtlVector<uint8_t>
  DWORD bSmokeVolumeDataReceived;  // bool
  uintptr_t bSmokeEffectSpawned;   // bool
};
extern SmokeGrenadeProjectileT SmokeGrenadeProjectile;

struct PawnT {
  DWORD BoneArray;
};
extern PawnT Pawn;

bool UpdateOffsets();
}  // namespace Offset
