#include "CS2x64.h"

#include <cstring>
#include <memory>

#include "Offsets.h"
#include "UkiaStuff.h"
#include "Utils/XorStr.h"
#include "Utils/vphys_parser/ray_trace.h"
#include "maths.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

inline size_t strlen_imp(const char* str) {
  size_t length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}
inline int strcmpi_imp(const char* s1, const char* s2) {
#if defined(_WIN32)
  return _stricmp(s1, s2);
#else
  return strcasecmp(s1, s2);
#endif
}

DWORD64 GetConvar(const char* name) {
  if (!name) return 0;
  DWORD64 cvar_interface = gGame.GetTier0DLLAddress() + Offset::VEngineCvar007;

  DWORD64 objs;
  Ukia::ProcessMgr.ReadMemory(cvar_interface + 64, objs);

  DWORD convar_length = strlen_imp(name);

  DWORD max_index;
  Ukia::ProcessMgr.ReadMemory(cvar_interface + 160, max_index);

  for (DWORD i = 0; i < max_index; i++) {
    DWORD64 entry;
    Ukia::ProcessMgr.ReadMemory(objs + 16 * i, entry);
    if (entry == 0) {
      break;
    }
    DWORD64 name_entry;
    char convar_name[120]{};
    Ukia::ProcessMgr.ReadMemory(entry + 0x00, name_entry);
    Ukia::ProcessMgr.ReadMemory(name_entry, convar_name, convar_length);
    convar_name[convar_length] = 0;

    if (!strcmpi_imp(convar_name, name)) {
      return entry;
    }
  }
  return 0;
}
template <typename ReadType>
bool GetConvarValue(const char* name, ReadType& Value) {
  DWORD64 convar = GetConvar(name);
  if (!convar) return false;
  return Ukia::ProcessMgr.ReadMemory(convar + 0x40, Value);
}

bool CBone::UpdateAllBoneData(const DWORD64& EntityPawnAddress) {
  if (EntityPawnAddress == 0) {
    return false;
  }
  this->EntityPawnAddress = EntityPawnAddress;

  DWORD64 GameSceneNode = 0;
  DWORD64 BoneArrayAddress = 0;
  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(
          EntityPawnAddress + Offset::C_BaseEntity.m_pGameSceneNode,
          GameSceneNode)) {
    return false;
  }
  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(
          GameSceneNode + Offset::Pawn.BoneArray, BoneArrayAddress)) {
    return false;
  }

  constexpr size_t NUM_BONES = 30;
  BoneJointData BoneArray[NUM_BONES]{};
  if (!Ukia::ProcessMgr.ReadMemory(BoneArrayAddress, BoneArray,
                                   NUM_BONES * sizeof(BoneJointData))) {
    return false;
  }

  BonePosList.clear();

  for (const auto& bone : BoneArray) {
    Vector2 ScreenPos;
    bool IsVisible = false;

    if (gGame.View.WorldToScreen(bone.Pos, ScreenPos)) {
      IsVisible = true;
    }

    this->BonePosList.push_back({bone.Pos, ScreenPos, IsVisible});
  }

  return !BonePosList.empty();
}
bool PlayerController::GetMoney() {
  DWORD64 MoneyServices;
  std::memcpy(&MoneyServices,
              this->ControllerBuffer.data() +
                  Offset::CCSPlayerController.m_pInGameMoneyServices,
              sizeof(MoneyServices));
  Ukia::ProcessMgr.GetDataAddressWithOffset<int>(
      MoneyServices, Offset::CCSPlayerController_InGameMoneyServices.m_iAccount,
      this->Money);
  Ukia::ProcessMgr.GetDataAddressWithOffset<int>(
      MoneyServices,
      Offset::CCSPlayerController_InGameMoneyServices.m_iTotalCashSpent,
      this->CashSpent);
  Ukia::ProcessMgr.GetDataAddressWithOffset<int>(
      MoneyServices,
      Offset::CCSPlayerController_InGameMoneyServices.m_iCashSpentThisRound,
      this->CashSpentTotal);
  return true;
}
bool PlayerController::GetTeamID() {
  std::memcpy(&this->TeamID,
              this->ControllerBuffer.data() + Offset::C_BaseEntity.m_iTeamNum,
              sizeof(this->TeamID));
  return true;
}
bool PlayerController::GetHealth() {
  std::memcpy(&this->Health,
              this->ControllerBuffer.data() + Offset::C_BaseEntity.m_iHealth,
              sizeof(this->Health));
  return true;
}
bool PlayerController::GetIsAlive() {
  std::memcpy(&this->AliveStatus,
              this->ControllerBuffer.data() +
                  Offset::CCSPlayerController.m_bPawnIsAlive,
              sizeof(this->AliveStatus));
  return true;
}
bool PlayerController::GetIsCtrlBot() {
  std::memcpy(&this->CtrlBot,
              this->ControllerBuffer.data() +
                  Offset::CCSPlayerController.m_bControllingBot,
              sizeof(this->CtrlBot));
  return true;
}
bool PlayerController::GetConnected() {
  std::memcpy(&this->Connected,
              this->ControllerBuffer.data() +
                  Offset::CCSPlayerController.m_bEverPlayedOnTeam,
              sizeof(this->Connected));
  return true;
}
bool PlayerController::GetHasHelmet() {
  std::memcpy(&this->HasHelmet,
              this->ControllerBuffer.data() +
                  Offset::CCSPlayerController.m_bPawnHasHelmet,
              sizeof(this->HasHelmet));
  return true;
}
bool PlayerController::GetPlayerSteamID() {
  std::memcpy(
      &this->SteamID,
      this->ControllerBuffer.data() + Offset::CBasePlayerController.m_steamID,
      sizeof(this->SteamID));
  return true;
}
bool PlayerController::GetPlayerName() {
  DWORD64 Addr;
  char Buffer[128]{};
  std::memcpy(&Addr,
              this->ControllerBuffer.data() +
                  Offset::CCSPlayerController.m_sSanitizedPlayerName,
              sizeof(Addr));
  if (!Ukia::ProcessMgr.ReadMemory(Addr, Buffer, 128)) return false;

  if (!this->SteamID)
    this->PlayerName = XorStr("BOT ") + std::string(Buffer);
  else
    this->PlayerName = Buffer;
  if (this->PlayerName.empty()) this->PlayerName = XorStr("Name_None");

  return true;
}
inline DWORD64 GethPawn(uint64_t Target) {
  DWORD64 EntityPawnListEntry = 0;
  DWORD64 EntityPawnAddress = 0;

  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(),
                                      EntityPawnListEntry))
    return 0;

  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(
          EntityPawnListEntry + 0x10 + 8 * ((Target & 0x7FFF) >> 9),
          EntityPawnListEntry))
    return 0;

  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(
          EntityPawnListEntry + 0x78 * (Target & 0x1FF), EntityPawnAddress))
    return 0;

  return EntityPawnAddress;
}
bool PlayerController::GetSpec() {
  
  uintptr_t pawn = this->GetPlayerhPawnAddress();

  uintptr_t obsService;
  Ukia::ProcessMgr.ReadMemory(
      pawn + Offset::C_BasePlayerPawn.m_pObserverServices, obsService);
  uint64_t obsTarget;
  Ukia::ProcessMgr.ReadMemory(
      obsService + Offset::CPlayer_ObserverServices.m_hObserverTarget,
      obsTarget);
  uintptr_t obsPawnHandle = GethPawn(obsTarget);

  if (obsPawnHandle == Vars::LocalPawnAddress)
  {
    this->IsSpec = true;
  }
  else
    this->IsSpec = false;
  return true;
}
DWORD64 PlayerController::GetPlayerPawnAddress() {
  DWORD64 EntityPawnListEntry = 0;
  DWORD64 EntityPawnAddress = 0;

  std::memcpy(
      &this->Pawn,
      this->ControllerBuffer.data() + Offset::CCSPlayerController.m_hPlayerPawn,
      sizeof(this->Pawn));

  return GethPawn(this->Pawn);
}

DWORD64 PlayerController::GetPlayerhPawnAddress() {
  DWORD64 EntityPawnListEntry = 0;
  DWORD64 EntityPawnAddress = 0;
  std::memcpy(
      &this->Pawn,
      this->ControllerBuffer.data() + Offset::CBasePlayerController.m_hPawn,
      sizeof(this->Pawn));

  return GethPawn(this->Pawn);
}

bool PlayerPawn::GetPos() {
  DWORD64 GameSceneNode;
  std::memcpy(&GameSceneNode,
              this->PawnBuffer.data() + Offset::C_BaseEntity.m_pGameSceneNode,
              sizeof(GameSceneNode));
  if (!Ukia::ProcessMgr.ReadMemory<bool>(
          GameSceneNode + Offset::CGameSceneNode.m_bDormant, this->IsDormanted))
    return false;

  if (!Ukia::ProcessMgr.ReadMemory<Vector3>(
          GameSceneNode + Offset::CGameSceneNode.m_vecOrigin, this->Pos))
    return false;
  gGame.View.WorldToScreen(this->Pos, this->ScreenPos);
  return true;
  // return GetDataAddressWithOffset<Vec3>(Address,
  // Offset::C_BasePlayerPawn.m_vOldOrigin, this->Pos);
}
bool PlayerPawn::GetHeight() {
  DWORD64 Collision;
  std::memcpy(&Collision,
              this->PawnBuffer.data() + Offset::C_BaseEntity.m_pCollision,
              sizeof(Collision));
  if (!Ukia::ProcessMgr.ReadMemory<Vector3>(
          Collision + Offset::CCollisionProperty.m_vecMins, this->MinPos))
    return false;
  if (!Ukia::ProcessMgr.ReadMemory<Vector3>(
          Collision + Offset::CCollisionProperty.m_vecMaxs, this->MaxPos))
    return false;
  this->Height = this->MaxPos.z - this->MinPos.z;
  this->Width = (Vector2{this->MaxPos.x, this->MaxPos.y} -
                 Vector2{this->MinPos.x, this->MinPos.y})
                    .Length();
  return true;
  // return GetDataAddressWithOffset<Vec3>(Address,
  // Offset::C_BasePlayerPawn.m_vOldOrigin, this->Pos);
}
bool PlayerPawn::GetViewAngle() {
  std::memcpy(
      &this->ViewAngle,
      this->PawnBuffer.data() + Offset::C_CSPlayerPawnBase.m_angEyeAngles,
      sizeof(this->ViewAngle));
  return true;
}

bool PlayerPawn::GetCameraPos() {
  std::memcpy(&this->CameraPos,
              this->PawnBuffer.data() +
                  Offset::C_CSPlayerPawnBase.m_vecLastClipCameraPos,
              sizeof(this->CameraPos));
  return true;
}
bool PlayerPawn::GetWeaponName() {
  DWORD64 WeaponNameAddress = 0;
  char Buffer[256]{};

  WeaponNameAddress = Ukia::ProcessMgr.TraceAddress(
      this->Address + Offset::C_CSPlayerPawnBase.m_pClippingWeapon,
      {0x10, 0x20, 0x0});
  if (WeaponNameAddress == 0) return false;

  auto sBuffer = Ukia::ProcessMgr.ReadString(WeaponNameAddress);

  if (!strstr(sBuffer.c_str(), "weapon_")) return false;

  sBuffer = sBuffer.substr(7);  // get rid of weapon_ prefix
  this->WeaponName = sBuffer;
  return true;
}

bool PlayerPawn::GetShotsFired() {
  std::memcpy(&this->ShotsFired,
              this->PawnBuffer.data() + Offset::C_CSPlayerPawn.m_iShotsFired,
              sizeof(this->ShotsFired));
  return true;
}

bool PlayerPawn::GetScoped() {
  std::memcpy(&this->Scoped,
              this->PawnBuffer.data() + Offset::C_CSPlayerPawn.m_bIsScoped,
              sizeof(this->Scoped));
  return true;
}

bool PlayerPawn::GetAimPunchAngle() {
  std::memcpy(&this->AimPunchAngle,
              this->PawnBuffer.data() + Offset::C_CSPlayerPawn.m_aimPunchAngle,
              sizeof(this->AimPunchAngle));
  return true;
}
bool PlayerPawn::GetHealth() {
  std::memcpy(&this->Health,
              this->PawnBuffer.data() + Offset::C_BaseEntity.m_iHealth,
              sizeof(this->Health));
  return true;
}
bool PlayerPawn::GetTeamID() {
  std::memcpy(&this->TeamID,
              this->PawnBuffer.data() + Offset::C_BaseEntity.m_iTeamNum,
              sizeof(this->TeamID));
  return true;
}
bool PlayerPawn::GetFov() {
  DWORD64 CameraServices = 0;
  std::memcpy(
      &CameraServices,
      this->PawnBuffer.data() + Offset::C_BasePlayerPawn.m_pCameraServices,
      sizeof(CameraServices));
  return Ukia::ProcessMgr.GetDataAddressWithOffset<int>(
      CameraServices, Offset::CCSPlayerBase_CameraServices.m_iFOVStart,
      this->Fov);
}
bool PlayerPawn::GetSpotted() {
  std::memcpy(&this->bSpottedByMask,
              this->PawnBuffer.data() + Offset::C_CSPlayerPawn.m_bSpottedByMask,
              sizeof(this->bSpottedByMask));
  return true;
}
bool PlayerPawn::GetFFlags() {
  std::memcpy(&this->fFlags,
              this->PawnBuffer.data() + Offset::C_BaseEntity.m_fFlags,
              sizeof(this->fFlags));
  return true;
}
bool PlayerPawn::GetAimPunchCache() {
  std::memcpy(&this->AimPunchCache,
              this->PawnBuffer.data() + Offset::C_CSPlayerPawn.m_aimPunchCache,
              sizeof(this->AimPunchCache));
  return true;
}
bool PlayerPawn::GetAmmo() {
  DWORD64 ClippingWeapon = 0;
  std::memcpy(
      &ClippingWeapon,
      this->PawnBuffer.data() + Offset::C_CSPlayerPawnBase.m_pClippingWeapon,
      sizeof(ClippingWeapon));

  return Ukia::ProcessMgr.GetDataAddressWithOffset<int>(
      ClippingWeapon, Offset::WeaponBaseData.Clip1, this->Ammo);
}

bool PlayerPawn::GetMaxAmmo() {
  DWORD64 ClippingWeapon = 0;
  DWORD64 WeaponData = 0;
  std::memcpy(
      &ClippingWeapon,
      this->PawnBuffer.data() + Offset::C_CSPlayerPawnBase.m_pClippingWeapon,
      sizeof(ClippingWeapon));
  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(
          ClippingWeapon + Offset::WeaponBaseData.WeaponDataPTR, WeaponData))
    return false;

  return Ukia::ProcessMgr.GetDataAddressWithOffset<int>(
      WeaponData, Offset::WeaponBaseData.MaxClip, this->MaxAmmo);
}
bool PlayerPawn::GetIsAuto() {
  DWORD64 ClippingWeapon = 0;
  DWORD64 WeaponData = 0;
  std::memcpy(
      &ClippingWeapon,
      this->PawnBuffer.data() + Offset::C_CSPlayerPawnBase.m_pClippingWeapon,
      sizeof(ClippingWeapon));
  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(
          ClippingWeapon + Offset::WeaponBaseData.WeaponDataPTR, WeaponData))
    return false;

  return Ukia::ProcessMgr.GetDataAddressWithOffset<bool>(
      WeaponData, Offset::WeaponBaseData.m_bIsFullAuto, this->IsAuto);
}
bool PlayerPawn::GetArmor() {
  std::memcpy(&this->Armor,
              this->PawnBuffer.data() + Offset::C_CSPlayerPawn.m_ArmorValue,
              sizeof(this->Armor));
  return true;
}
bool PlayerPawn::GetFlashDuration() {
  std::memcpy(
      &this->FlashDuration,
      this->PawnBuffer.data() + Offset::C_CSPlayerPawnBase.m_flFlashDuration,
      sizeof(this->FlashDuration));
  return true;
}
bool PlayerPawn::GetIsImmunity() {
  std::memcpy(&this->isImmunity,
      this->PawnBuffer.data() + Offset::C_CSPlayerPawnBase.m_bGunGameImmunity,
              sizeof(this->isImmunity));
  return true;
}
bool PlayerPawn::GetVelocity() {
  std::memcpy(&this->Velocity,
              this->PawnBuffer.data() + Offset::C_BaseEntity.m_vecAbsVelocity,
              sizeof(this->Velocity));
  this->Speed = sqrt(this->Velocity.x * this->Velocity.x +
                     this->Velocity.y * this->Velocity.y);
  return true;
}

bool PlantedC4::GetIsPlanted() {
  std::memcpy(&this->isPlanted,
              this->c4Buffer.data() + Offset::C_PlantedC4.m_bC4Activated,
              sizeof(this->isPlanted));
  return true;
}
bool PlantedC4::GetIsDefusing() {
  std::memcpy(&this->isDefusing,
              this->c4Buffer.data() + Offset::C_PlantedC4.m_bBeingDefused,
              sizeof(this->isDefusing));
  return true;
}
bool PlantedC4::GetHasExploded() {
  std::memcpy(&this->hasExploded,
              this->c4Buffer.data() + Offset::C_PlantedC4.m_bHasExploded,
              sizeof(this->hasExploded));
  return true;
}
bool PlantedC4::GetBoomTime() {
  std::memcpy(&this->boomTime,
              this->c4Buffer.data() + Offset::C_PlantedC4.m_flC4Blow,
              sizeof(this->boomTime));
  return true;
}

bool PlantedC4::GetDefuseTime() {
  std::memcpy(&this->defuseTime,
              this->c4Buffer.data() + Offset::C_PlantedC4.m_flDefuseCountDown,
              sizeof(this->defuseTime));
  return true;
}
bool PlantedC4::GetBoomRemaining() {
  if (!this->isPlanted)
    this->boomRemaining = 0;
  else
    this->boomRemaining = this->boomTime - Vars::CurTime;
  return true;
}
bool PlantedC4::GetDefuseRemaining() {
  if (!this->isDefusing)
    this->defuseRemaining = 0;
  else
    this->defuseRemaining = this->defuseTime - Vars::CurTime;
  return true;
}
bool PlantedC4::GetBombSite() {
  std::memcpy(&this->bombSite,
              this->c4Buffer.data() + Offset::C_PlantedC4.m_nBombSite,
              sizeof(this->bombSite));
  return true;
}
bool PlantedC4::GetBombPos() {
  DWORD64 GameSceneNode;
  std::memcpy(&GameSceneNode,
              this->c4Buffer.data() + Offset::C_BaseEntity.m_pGameSceneNode,
              sizeof(GameSceneNode));
  if (!Ukia::ProcessMgr.ReadMemory<Vector3>(
          GameSceneNode + Offset::CGameSceneNode.m_vecOrigin, this->Pos))
    this->Pos = {0, 0, 0};
  return true;
}

bool PlantedC4::UpdatePlantedC4(const DWORD64& PlantedC4Address) {
  if (PlantedC4Address == 0) return false;
  this->Address = PlantedC4Address;
  Ukia::ProcessMgr.ReadMemory(this->Address, this->c4Buffer);

  if (!this->GetIsPlanted()) return false;
  if (!this->GetIsDefusing()) return false;
  if (!this->GetHasExploded()) return false;
  if (!this->GetBoomTime()) return false;
  if (!this->GetDefuseTime()) return false;
  if (!this->GetBoomRemaining()) return false;
  if (!this->GetDefuseRemaining()) return false;
  if (!this->GetBombSite()) return false;
  if (!this->GetBombPos()) return false;
  return true;
}

// 更新数据
bool CEntity::UpdateController(const DWORD64& PlayerControllerAddress) {
  if (PlayerControllerAddress == 0) return false;
  this->Controller.Address = PlayerControllerAddress;
  Ukia::ProcessMgr.ReadMemory(this->Controller.Address,
                              this->Controller.ControllerBuffer);
  if (!this->Controller.GetHealth()) return false;
  if (!this->Controller.GetIsAlive()) return false;
  if (!this->Controller.GetIsCtrlBot()) return false;
  if (!this->Controller.GetConnected()) return false;
  if (!this->Controller.GetHasHelmet()) return false;
  if (!this->Controller.GetTeamID()) return false;
  if (!this->Controller.GetPlayerSteamID()) return false;
  if (!this->Controller.GetPlayerName()) return false;
  if (!this->Controller.GetSpec()) return false;
  if (!this->Controller.GetMoney()) return false;

  this->Pawn.Address =
      this->Controller
          .GetPlayerhPawnAddress();  // this->Controller.GetPlayerPawnAddress();
  return true;
}
bool CEntity::UpdatePawn(const DWORD64& PlayerPawnAddress) {
  if (PlayerPawnAddress == 0) return false;
  this->Pawn.Address = PlayerPawnAddress;
  Ukia::ProcessMgr.ReadMemory(this->Pawn.Address, this->Pawn.PawnBuffer);
  if (!this->Pawn.GetCameraPos()) return false;
  if (!this->Pawn.GetPos()) return false;
  if (!this->Pawn.GetHeight()) return false;
  if (!this->Pawn.GetViewAngle()) return false;
  if (!this->Pawn.GetWeaponName()) return false;
  if (!this->Pawn.GetAimPunchAngle()) return false;
  if (!this->Pawn.GetShotsFired()) return false;
  if (!this->Pawn.GetScoped()) return false;
  if (!this->Pawn.GetHealth()) return false;
  if (!this->Pawn.GetAmmo()) return false;
  if (!this->Pawn.GetMaxAmmo()) return false;
  if (!this->Pawn.GetIsAuto()) return false;
  if (!this->Pawn.GetArmor()) return false;
  if (!this->Pawn.GetTeamID()) return false;
  if (!this->Pawn.GetFov()) return false;
  if (!this->Pawn.GetSpotted()) return false;
  if (!this->Pawn.GetFFlags()) return false;
  if (!this->Pawn.GetFlashDuration()) return false;
  if (!this->Pawn.GetIsImmunity()) return false;
  if (!this->Pawn.GetVelocity()) return false;
  if (!this->Pawn.GetAimPunchCache()) return false;
  if (!this->Pawn.BoneData.UpdateAllBoneData(PlayerPawnAddress)) return false;

  return true;
}

bool CEntity::IsEnemy() {
  if (!config::TeamCheck || Vars::FreeFire) return true;
  if (this->Controller.TeamID == Vars::LocalEntity.Controller.TeamID)
    return false;
  if (this->Pawn.TeamID == Vars::LocalEntity.Pawn.TeamID) return false;
  return true;
}

bool CEntity::IsVisible() {
  if (!this->IsInScreen()) return false;
  if (Vars::IsMapFileExist) {
    bool Visible;
    std::lock_guard<std::mutex> lock(Vars::VisibleEntityAddrMutex);
    bool ParserVisible =
        std::find(Vars::VisibleEntityAddr.begin(),
                  Vars::VisibleEntityAddr.end(),
                  this->Controller.Address) != Vars::VisibleEntityAddr.end();
    bool SpottedVisible = this->Pawn.bSpottedByMask;
    return (ParserVisible /* || SpottedVisible*/);
  } else {
    return this->Pawn.bSpottedByMask;
  }
}


namespace Vars {

map_loader ParsingMap;
std::string ParsingMapName = {};

void ParserRun() noexcept {
  if (!IsInGame) return;

  std::string mapPath = config::path + XorStr("\\Maps\\") + MapName;
  if (MapName != ParsingMapName) {
    if (IsMapFileExist) {
      ParsingMap.unload();
      IsMapFileExist = false;
    }
    ParsingMapName = MapName;
    ParsingMap.load_map(mapPath);
    IsMapFileExist = true;
  }
  std::vector<DWORD64> tempVisibleEntityAddr;
  std::lock_guard<std::mutex> lock(validEntityMutex);
  for (int index = 0; index < ValidEntity.size(); index++) {
    CEntity Entity = ValidEntity[index].first;
    DWORD64 EntityAddress = ValidEntity[index].second;

    if (!Entity.IsEnemy()) continue;  // might someday remove this

    if (Entity.Pawn.IsDormanted) continue;

    if (!Entity.ESPAlive()) continue;

    if (!Entity.IsInScreen()) continue;
    Vector r_start = Vector(Vars::LocalEntity.Pawn.CameraPos.x,
                            Vars::LocalEntity.Pawn.CameraPos.y,
                            Vars::LocalEntity.Pawn.CameraPos.z);
    Vector r_end = Vector(Entity.Pawn.Pos.x, Entity.Pawn.Pos.y,
                          Entity.Pawn.Pos.z + 0.45 * Entity.Pawn.Height);
    if (!ParsingMap.is_visible(r_start, r_end)) continue;
    tempVisibleEntityAddr.push_back(EntityAddress);
  }
  std::lock_guard<std::mutex> lock0(VisibleEntityAddrMutex);
  VisibleEntityAddr.clear();
  VisibleEntityAddr.reserve(tempVisibleEntityAddr.size());
  for (DWORD64 theVisibleEntityAddress : tempVisibleEntityAddr) {
    VisibleEntityAddr.emplace_back(theVisibleEntityAddress);
  }
}

inline void InGameCheck() {
  /*
    DWORD game_state;
  Ukia::ProcessMgr.ReadMemory(gGame.GetEngineDLLAddress()+
  Offset::dwNetworkGameClient_signOnState, game_state); std::cout << game_state
  << std::endl; if (game_state != 6) { IsInGame = false; return;
    }

    */
  if (MapName.empty() || MapName == XorStr("<empty>") ||
      MapName == XorStr("SNDLVL_35dB")) {
    IsInGame = false;
    return;
  }
  IsInGame = true;
  return;
}

inline bool GetSensitivity() {
  DWORD64 dwSensitivity;
  float flSensitivity;
  Ukia::ProcessMgr.ReadMemory(
      gGame.GetClientDLLAddress() + Offset::dwSensitivity, dwSensitivity);
  if (Ukia::ProcessMgr.ReadMemory(dwSensitivity + Offset::Sensitivity,
                                  flSensitivity)) {
    Sensitivity = flSensitivity;
    return true;
  } else
    return false;
}
void UpdateDataSlow() {
  if (!Ukia::ProcessMgr.ReadMemory<CGlobalVarsBase>(
          gGame.GetGlobalVarsAddress(), Global_Vars))
    return;
  MapName = Ukia::ProcessMgr.ReadString(Global_Vars.m_current_mapname, 32);
  CurTime = Global_Vars.m_curtime;
  TickCount = Global_Vars.m_tickcount;
  FrameTime = Global_Vars.m_frametime;
  UpdateIntervals();
  GetConvarValue(XorStr("mp_teammates_are_enemies"), Vars::FreeFire);
  InGameCheck();
  GetSensitivity();
  if (!IsInGame) return;

  DWORD64 PlantedC4PTRAddress = 0;
  if (!Ukia::ProcessMgr.ReadMemory(gGame.GetPlantedC4PTRAddress(),
                                   PlantedC4PTRAddress))
    return;
  if (!Ukia::ProcessMgr.ReadMemory(PlantedC4PTRAddress, PlantedC4Address))
    return;
  if (!PlantedBomb.UpdatePlantedC4(PlantedC4Address)) return;
}
void UpdateData() {
  if (!global::isFocused) return;
  if (!Ukia::ProcessMgr.ReadMemory(gGame.GetMatrixAddress(), gGame.View.Matrix,
                                   64))
    return;

  if (!gGame.UpdateEntityListEntry()) return;
  if (!Ukia::ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(),
                                   LocalControllerAddress))
    return;
  if (!Ukia::ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(),
                                   LocalPawnAddress))
    return;
  if (!LocalEntity.UpdateController(LocalControllerAddress)) return;
  if (!LocalEntity.UpdatePawn(LocalPawnAddress) && !config::WorkInSpec) return;


  if (global::userName == std::getenv(XorStr("USERNAME")))
    global::userName = LocalEntity.Controller.PlayerName;
  if (!IsInGame) {
    std::lock_guard<std::mutex> lock(validEntityMutex);
    ValidEntity.clear();
    return;
  }

  std::vector<std::pair<std::shared_ptr<CEntity>, DWORD64>> tempValidEntity;

  constexpr int MAX_ENTITY = 64;
  constexpr int ENTITY_STRIDE = 0x78;
  std::array<char, MAX_ENTITY * ENTITY_STRIDE> entityAddresses{};

  Ukia::ProcessMgr.ReadMemory(gGame.GetEntityListEntry() + 0x78,
                              entityAddresses, MAX_ENTITY * ENTITY_STRIDE);
#pragma omp parallel for
  for (int i = 0; i <= MAX_ENTITY; i++) {
    DWORD64 EntityAddress;
    std::memcpy(&EntityAddress, entityAddresses.data() + i * ENTITY_STRIDE,
                sizeof(DWORD64));

    if (EntityAddress == 0) continue;
    if (EntityAddress == LocalEntity.Controller.Address) {
      LocalPlayerControllerIndex = i;
      continue;
    }
    auto Entity = std::make_shared<CEntity>();
    if (!Entity->UpdateController(EntityAddress)) continue;
    Entity->UpdatePawn(Entity->Pawn.Address);
    // here,grab it.
    tempValidEntity.push_back(std::make_pair(Entity, EntityAddress));
  }
  std::lock_guard<std::mutex> lock(validEntityMutex);
  ValidEntity.clear();
  ValidEntity.reserve(tempValidEntity.size());

  for (auto& pair : tempValidEntity) {
    ValidEntity.emplace_back(*pair.first, pair.second);
  }
}

void UpdateIntervals()
{
  RenderInterval = 
      config::RenderInterval ? config::RenderInterval : std::floor(FrameTime);
  GlobalVarsInterval = 
      config::GlobalVarsInterval ? config::GlobalVarsInterval : std::floor(FrameTime);
  EntityInterval =
      config::EntityInterval ? config::EntityInterval : std::floor(FrameTime);
  ParserInterval =
      config::ParserInterval ? config::ParserInterval : std::floor(FrameTime);
  AimInterval =
      config::AimInterval ? config::AimInterval : std::floor(FrameTime);
  ViewInterval =
      config::ViewInterval ? config::ViewInterval : std::floor(FrameTime);
  MemoryInterval =
      config::MemoryInterval ? config::MemoryInterval : std::floor(FrameTime);
  NonMemoryInterval =
      config::NonMemoryInterval ? config::NonMemoryInterval : std::floor(FrameTime);
}

}  // namespace Vars
