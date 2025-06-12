#include "Offsets.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "Functions/config.h"
#include "UkiaStuff.h"
#include "Utils/XorStr.h"

namespace Offset {

uintptr_t ForceJump = 0;
uintptr_t ForceCrouch = 0;
uintptr_t ForceForward = 0;
uintptr_t ForceLeft = 0;
uintptr_t ForceRight = 0;

uintptr_t EntityList = 0;
uintptr_t Matrix = 0;
uintptr_t ViewAngle = 0;
uintptr_t LocalPlayerController = 0;
uintptr_t LocalPlayerPawn = 0;
uintptr_t GlobalVars = 0;
uintptr_t CSGOInput = 0;
uintptr_t InventoryServices = 0;
uintptr_t PlantedC4 = 0;
uintptr_t InputSystem = 0;
uintptr_t dwSensitivity = 0;
uintptr_t Sensitivity = 0;
uintptr_t BuildNumber = 0;
uintptr_t dwNetworkGameClient_signOnState = 0;

uintptr_t VEngineCvar007 = 0;

C_BaseEntityT C_BaseEntity = {};

C_BaseModelEntityT C_BaseModelEntity = {};

CGameSceneNodeT CGameSceneNode = {};

CCollisionPropertyT CCollisionProperty = {};

CBasePlayerControllerT CBasePlayerController = {};

CCSPlayerControllerT CCSPlayerController = {};

C_BasePlayerPawnT C_BasePlayerPawn = {};

C_CSPlayerPawnBaseT C_CSPlayerPawnBase = {};

C_CSPlayerPawnT C_CSPlayerPawn = {};

C_PlantedC4T C_PlantedC4 = {};

CPlayer_ObserverServicesT CPlayer_ObserverServices = {};

CCSPlayer_ViewModelServicesT CCSPlayer_ViewModelServices = {};

CPlayer_MovementServices_HumanoidT CPlayer_MovementServices_Humanoid = {};

CCSPlayerBase_CameraServicesT CCSPlayerBase_CameraServices = {};

CCSPlayer_BulletServicesT CCSPlayer_BulletServices = {};

CCSPlayerController_InGameMoneyServicesT
    CCSPlayerController_InGameMoneyServices = {};

EconEntityT EconEntity = {};

WeaponBaseDataT WeaponBaseData = {};

SmokeGrenadeProjectileT SmokeGrenadeProjectile = {};

PawnT Pawn = {};

}  // namespace Offset

using json = nlohmann::json;

namespace fs = std::filesystem;
/*
DWORD64 SearchOffsets(std::string Signature, DWORD64 ModuleAddress)
{
        std::vector<DWORD64> TempAddressList;
        DWORD64 Address = 0;
        DWORD Offsets = 0;

        TempAddressList = ProcessMgr.SearchMemory(Signature, ModuleAddress,
ModuleAddress + 0x4000000);

        if (TempAddressList.size() <= 0)
                return 0;

        if (!ProcessMgr.ReadMemory<DWORD>(TempAddressList.at(0) + 3, Offsets))
                return 0;

        Address = TempAddressList.at(0) + Offsets + 7;
        return Address;
}
*/
template <typename T>
inline static T ReadJsonData(const nlohmann::json& node, T defaultValue) {
  return node.is_null() ? defaultValue : node.get<T>();
}

bool Offset::UpdateOffsets() {
  std::string currentDir = fs::current_path().string();
  std::string offsetFileName = "offsets.json";
  std::string buttonFileName = "buttons.json";
  std::string clientFileName = "client_dll.json";
  std::string interfaceFileName = "interfaces.json";
  std::string offsetsDir = config::path + XorStr("\\Offsets\\");

  struct FilePair {
    std::string src;
    std::string dst;
    const char* name;
  };

  FilePair files[] = {{currentDir + "\\" + offsetFileName,
                       offsetsDir + offsetFileName, "offsets.json"},
                      {currentDir + "\\" + buttonFileName,
                       offsetsDir + buttonFileName, "buttons.json"},
                      {currentDir + "\\" + clientFileName,
                       offsetsDir + clientFileName, "client_dll.json"},
                      {currentDir + "\\" + interfaceFileName,
                       offsetsDir + interfaceFileName, "interfaces.json"}};

  bool anyCopied = false;
  for (const auto& file : files) {
    if (fs::exists(file.src)) {
      try {
        fs::copy_file(file.src, file.dst, fs::copy_options::overwrite_existing);
        printf(XorStr("Copied and overwrote %s\n"), file.name);
        anyCopied = true;
      } catch (const std::exception& e) {
        printf(XorStr("Error copying %s: %s\n"), file.name, e.what());
      }
    }
  }

  // 检查目标文件是否都存在
  if (!(fs::exists(files[0].dst) && fs::exists(files[1].dst) &&
        fs::exists(files[2].dst) && fs::exists(files[3].dst))) {
    printf(
        XorStr("Failed to locate all offsets file. Please visit "
               "https://github.com/a2x/cs2-dumper/tree/main/output to get "
               "latest offsets.json & buttons.json & client_dll.json & "
               "interfaces.json\n"));
    return false;
  }

  Offset::WeaponBaseData.WeaponSize = 0x50;

  Offset::Pawn.BoneArray = 0x1F0;

  std::ifstream offsetfile(files[0].dst);
  nlohmann::json offset;
  offsetfile >> offset;

  Offset::EntityList = ReadJsonData(offset["client.dll"]["dwEntityList"], 0);

  Offset::LocalPlayerController =
      ReadJsonData(offset["client.dll"]["dwLocalPlayerController"], 0);
  Offset::Matrix = ReadJsonData(offset["client.dll"]["dwViewMatrix"], 0);
  Offset::GlobalVars = ReadJsonData(offset["client.dll"]["dwGlobalVars"], 0);
  Offset::CSGOInput = ReadJsonData(offset["client.dll"]["dwCSGOInput"], 0);
  Offset::ViewAngle = ReadJsonData(offset["client.dll"]["dwViewAngles"], 0);
  Offset::LocalPlayerPawn =
      ReadJsonData(offset["client.dll"]["dwLocalPlayerPawn"], 0);
  Offset::PlantedC4 = ReadJsonData(offset["client.dll"]["dwPlantedC4"], 0);
  Offset::dwSensitivity =
      ReadJsonData(offset["client.dll"]["dwSensitivity"], 0);
  Offset::Sensitivity =
      ReadJsonData(offset["client.dll"]["dwSensitivity_sensitivity"], 0);
  Offset::BuildNumber = ReadJsonData(offset["engine2.dll"]["dwBuildNumber"], 0);
  Offset::dwNetworkGameClient_signOnState =
      ReadJsonData(offset["engine2.dll"]["dwNetworkGameClient_signOnState"], 0);
  Offset::InputSystem =
      ReadJsonData(offset["inputsystem.dll"]["dwInputSystem"], 0);

  std::ifstream interfacesfile(files[3].dst);
  nlohmann::json interfaces;
  interfacesfile >> interfaces;
  Offset::VEngineCvar007 =
      ReadJsonData(interfaces["tier0.dll"]["VEngineCvar007"], 0);

  std::ifstream buttonfile(files[1].dst);
  nlohmann::json button;
  buttonfile >> button;
  Offset::ForceJump = ReadJsonData(button["client.dll"]["jump"], 0);
  Offset::ForceCrouch = ReadJsonData(button["client.dll"]["duck"], 0);
  Offset::ForceForward = ReadJsonData(button["client.dll"]["forward"], 0);
  Offset::ForceLeft = ReadJsonData(button["client.dll"]["left"], 0);
  Offset::ForceRight = ReadJsonData(button["client.dll"]["right"], 0);

  std::ifstream clientfile(files[2].dst);
  nlohmann::json client;
  clientfile >> client;
  Offset::C_BaseEntity.m_flGravityScale =
      ReadJsonData(client["client.dll"]["classes"]["C_BaseEntity"]["fields"]
                         ["m_flGravityScale"],
                   0);
  Offset::C_BaseEntity.m_iMaxHealth = ReadJsonData(
      client["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_iMaxHealth"],
      0);
  Offset::C_BaseEntity.m_iHealth = ReadJsonData(
      client["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_iHealth"],
      0);
  Offset::C_BaseEntity.m_pGameSceneNode =
      ReadJsonData(client["client.dll"]["classes"]["C_BaseEntity"]["fields"]
                         ["m_pGameSceneNode"],
                   0);
  Offset::C_BaseEntity.m_pCollision = ReadJsonData(
      client["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_pCollision"],
      0);
  Offset::C_BaseEntity.m_vecAbsVelocity =
      ReadJsonData(client["client.dll"]["classes"]["C_BaseEntity"]["fields"]
                         ["m_vecAbsVelocity"],
                   0);
  Offset::C_BaseEntity.m_iTeamNum = ReadJsonData(
      client["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_iTeamNum"],
      0);
  Offset::C_BaseEntity.m_fFlags = ReadJsonData(
      client["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_fFlags"], 0);

  Offset::C_BaseModelEntity.GlowFunction =
      ReadJsonData(client["client.dll"]["classes"]["C_BaseModelEntity"]
                         ["fields"]["m_Glow"],
                   0) +
      ReadJsonData(client["client.dll"]["classes"]["CGlowProperty"]["fields"]
                         ["m_bGlowing"],
                   0);
  Offset::C_BaseModelEntity.GlowColorOverride =
      ReadJsonData(client["client.dll"]["classes"]["C_BaseModelEntity"]
                         ["fields"]["m_Glow"],
                   0) +
      ReadJsonData(client["client.dll"]["classes"]["CGlowProperty"]["fields"]
                         ["m_glowColorOverride"],
                   0);

  Offset::CGameSceneNode.m_vecOrigin =
      ReadJsonData(client["client.dll"]["classes"]["CGameSceneNode"]["fields"]
                         ["m_vecOrigin"],
                   0);
  Offset::CGameSceneNode.m_bDormant = ReadJsonData(
      client["client.dll"]["classes"]["CGameSceneNode"]["fields"]["m_bDormant"],
      0);

  Offset::CCollisionProperty.m_vecMins =
      ReadJsonData(client["client.dll"]["classes"]["CCollisionProperty"]
                         ["fields"]["m_vecMins"],
                   0);
  Offset::CCollisionProperty.m_vecMaxs =
      ReadJsonData(client["client.dll"]["classes"]["CCollisionProperty"]
                         ["fields"]["m_vecMaxs"],
                   0);

  Offset::CBasePlayerController.m_hPawn =
      ReadJsonData(client["client.dll"]["classes"]["CBasePlayerController"]
                         ["fields"]["m_hPawn"],
                   0);
  Offset::CBasePlayerController.m_iszPlayerName =
      ReadJsonData(client["client.dll"]["classes"]["CBasePlayerController"]
                         ["fields"]["m_iszPlayerName"],
                   0);
  Offset::CBasePlayerController.m_iDesiredFOV =
      ReadJsonData(client["client.dll"]["classes"]["CBasePlayerController"]
                         ["fields"]["m_iDesiredFOV"],
                   0);
  Offset::CBasePlayerController.m_steamID =
      ReadJsonData(client["client.dll"]["classes"]["CBasePlayerController"]
                         ["fields"]["m_steamID"],
                   0);

  Offset::CCSPlayerController.m_bPawnIsAlive =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_bPawnIsAlive"],
                   0);
  Offset::CCSPlayerController.m_bControllingBot =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_bControllingBot"],
                   0);
  Offset::CCSPlayerController.m_bEverPlayedOnTeam =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_bEverPlayedOnTeam"],
                   0);
  Offset::CCSPlayerController.m_hPlayerPawn =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_hPlayerPawn"],
                   0);
  Offset::CCSPlayerController.m_sSanitizedPlayerName =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_sSanitizedPlayerName"],
                   0);
  Offset::CCSPlayerController.m_iPawnArmor =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_iPawnArmor"],
                   0);
  Offset::CCSPlayerController.m_bPawnHasDefuser =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_bPawnHasDefuser"],
                   0);
  Offset::CCSPlayerController.m_bPawnHasHelmet =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_bPawnHasHelmet"],
                   0);
  Offset::CCSPlayerController.m_iPing =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_iPing"],
                   0);
  Offset::CCSPlayerController.m_pInGameMoneyServices =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayerController"]
                         ["fields"]["m_pInGameMoneyServices"],
                   0);

  Offset::C_BasePlayerPawn.m_pMovementServices =
      ReadJsonData(client["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]
                         ["m_pMovementServices"],
                   0);
  Offset::C_BasePlayerPawn.m_pWeaponServices =
      ReadJsonData(client["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]
                         ["m_pWeaponServices"],
                   0);
  Offset::C_BasePlayerPawn.m_pCameraServices =
      ReadJsonData(client["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]
                         ["m_pCameraServices"],
                   0);
  Offset::C_BasePlayerPawn.m_pObserverServices =
      ReadJsonData(client["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]
                         ["m_pObserverServices"],
                   0);
  Offset::C_BasePlayerPawn.m_vOldOrigin =
      ReadJsonData(client["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]
                         ["m_vOldOrigin"],
                   0);
  Offset::C_BasePlayerPawn.m_hController =
      ReadJsonData(client["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]
                         ["m_hController"],
                   0);

  Offset::C_CSPlayerPawnBase.m_pViewModelServices =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_pViewModelServices"],
                   0);
  Offset::C_CSPlayerPawnBase.m_pClippingWeapon =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_pClippingWeapon"],
                   0);
  Offset::C_CSPlayerPawnBase.m_angEyeAngles =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_angEyeAngles"],
                   0);
  Offset::C_CSPlayerPawnBase.m_bGunGameImmunity =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_bGunGameImmunity"],
                   0);
  Offset::C_CSPlayerPawnBase.m_vecLastClipCameraPos =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_vecLastClipCameraPos"],
                   0);
  Offset::C_CSPlayerPawnBase.m_flFlashMaxAlpha =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_flFlashMaxAlpha"],
                   0);
  Offset::C_CSPlayerPawnBase.m_flFlashDuration =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_flFlashDuration"],
                   0);
  Offset::C_CSPlayerPawnBase.m_iIDEntIndex =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawnBase"]
                         ["fields"]["m_iIDEntIndex"],
                   0);

  Offset::C_CSPlayerPawn.m_pBulletServices =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_pBulletServices"],
                   0);
  Offset::C_CSPlayerPawn.m_bIsScoped =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_bIsScoped"],
                   0);
  Offset::C_CSPlayerPawn.m_bIsDefusing =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_bIsDefusing"],
                   0);
  Offset::C_CSPlayerPawn.m_ArmorValue =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_ArmorValue"],
                   0);
  Offset::C_CSPlayerPawn.m_iShotsFired =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_iShotsFired"],
                   0);
  Offset::C_CSPlayerPawn.m_aimPunchAngle =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_aimPunchAngle"],
                   0);
  Offset::C_CSPlayerPawn.m_aimPunchCache =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_aimPunchCache"],
                   0);
  Offset::C_CSPlayerPawn.m_bIsBuyMenuOpen =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_bIsBuyMenuOpen"],
                   0);
  Offset::C_CSPlayerPawn.m_bWaitForNoAttack =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_bWaitForNoAttack"],
                   0);
  Offset::C_CSPlayerPawn.m_bSpottedByMask =
      ReadJsonData(client["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]
                         ["m_entitySpottedState"],
                   0) +
      ReadJsonData(client["client.dll"]["classes"]["EntitySpottedState_t"]
                         ["fields"]["m_bSpottedByMask"],
                   0);

  Offset::C_PlantedC4.m_flC4Blow = ReadJsonData(
      client["client.dll"]["classes"]["C_PlantedC4"]["fields"]["m_flC4Blow"],
      0);
  Offset::C_PlantedC4.m_bC4Activated =
      ReadJsonData(client["client.dll"]["classes"]["C_PlantedC4"]["fields"]
                         ["m_bC4Activated"],
                   0);
  Offset::C_PlantedC4.m_bBeingDefused =
      ReadJsonData(client["client.dll"]["classes"]["C_PlantedC4"]["fields"]
                         ["m_bBeingDefused"],
                   0);
  Offset::C_PlantedC4.m_bHasExploded =
      ReadJsonData(client["client.dll"]["classes"]["C_PlantedC4"]["fields"]
                         ["m_bHasExploded"],
                   0);
  Offset::C_PlantedC4.m_hBombDefuser =
      ReadJsonData(client["client.dll"]["classes"]["C_PlantedC4"]["fields"]
                         ["m_hBombDefuser"],
                   0);
  Offset::C_PlantedC4.m_flDefuseCountDown =
      ReadJsonData(client["client.dll"]["classes"]["C_PlantedC4"]["fields"]
                         ["m_flDefuseCountDown"],
                   0);
  Offset::C_PlantedC4.m_nBombSite = ReadJsonData(
      client["client.dll"]["classes"]["C_PlantedC4"]["fields"]["m_nBombSite"],
      0);

  Offset::CPlayer_ObserverServices.m_hObserverTarget =
      ReadJsonData(client["client.dll"]["classes"]["CPlayer_ObserverServices"]
                         ["fields"]["m_hObserverTarget"],
                   0);
  Offset::CPlayer_ObserverServices.m_iObserverMode =
      ReadJsonData(client["client.dll"]["classes"]["CPlayer_ObserverServices"]
                         ["fields"]["m_iObserverMode"],
                   0);
  Offset::CCSPlayer_ViewModelServices.m_hViewModel = ReadJsonData(
      client["client.dll"]["classes"]["CCSPlayer_ViewModelServices"]["fields"]
            ["m_hViewModel"],
      0);
  Offset::CPlayer_MovementServices_Humanoid.m_nCrouchState = ReadJsonData(
      client["client.dll"]["classes"]["CPlayer_MovementServices_Humanoid"]
            ["fields"]["m_nCrouchState"],
      0);
  Offset::CCSPlayerBase_CameraServices.m_iFOVStart = ReadJsonData(
      client["client.dll"]["classes"]["CCSPlayerBase_CameraServices"]["fields"]
            ["m_iFOVStart"],
      0);
  Offset::CCSPlayer_BulletServices.m_totalHitsOnServer =
      ReadJsonData(client["client.dll"]["classes"]["CCSPlayer_BulletServices"]
                         ["fields"]["m_totalHitsOnServer"],
                   0);

  Offset::CCSPlayerController_InGameMoneyServices.m_iAccount = ReadJsonData(
      client["client.dll"]["classes"]["CCSPlayerController_InGameMoneyServices"]
            ["fields"]["m_iAccount"],
      0);
  Offset::CCSPlayerController_InGameMoneyServices
      .m_iTotalCashSpent = ReadJsonData(
      client["client.dll"]["classes"]["CCSPlayerController_InGameMoneyServices"]
            ["fields"]["m_iTotalCashSpent"],
      0);
  Offset::CCSPlayerController_InGameMoneyServices
      .m_iCashSpentThisRound = ReadJsonData(
      client["client.dll"]["classes"]["CCSPlayerController_InGameMoneyServices"]
            ["fields"]["m_iCashSpentThisRound"],
      0);

  Offset::WeaponBaseData.WeaponDataPTR =
      ReadJsonData(client["client.dll"]["classes"]["C_BaseEntity"]["fields"]
                         ["m_nSubclassID"],
                   0) +
      0x08;
  Offset::WeaponBaseData.szName =
      ReadJsonData(client["client.dll"]["classes"]["CCSWeaponBaseVData"]
                         ["fields"]["m_szName"],
                   0);
  Offset::WeaponBaseData.Clip1 =
      ReadJsonData(client["client.dll"]["classes"]["C_BasePlayerWeapon"]
                         ["fields"]["m_iClip1"],
                   0);
  Offset::WeaponBaseData.MaxClip =
      ReadJsonData(client["client.dll"]["classes"]["CBasePlayerWeaponVData"]
                         ["fields"]["m_iMaxClip1"],
                   0);
  Offset::WeaponBaseData.CycleTime =
      ReadJsonData(client["client.dll"]["classes"]["CCSWeaponBaseVData"]
                         ["fields"]["m_flCycleTime"],
                   0);
  Offset::WeaponBaseData.Penetration =
      ReadJsonData(client["client.dll"]["classes"]["CCSWeaponBaseVData"]
                         ["fields"]["m_flPenetration"],
                   0);
  Offset::WeaponBaseData.WeaponType =
      ReadJsonData(client["client.dll"]["classes"]["CCSWeaponBaseVData"]
                         ["fields"]["m_WeaponType"],
                   0);
  Offset::WeaponBaseData.Inaccuracy =
      ReadJsonData(client["client.dll"]["classes"]["CCSWeaponBaseVData"]
                         ["fields"]["m_flInaccuracyMove"],
                   0);
  Offset::WeaponBaseData.inReload =
      ReadJsonData(client["client.dll"]["classes"]["C_CSWeaponBase"]["fields"]
                         ["m_bInReload"],
                   0);
  Offset::WeaponBaseData.m_nNumBullets =
      ReadJsonData(client["client.dll"]["classes"]["CCSWeaponBaseVData"]
                         ["fields"]["m_nNumBullets"],
                   0);
  Offset::WeaponBaseData.ActiveWeapon =
      ReadJsonData(client["client.dll"]["classes"]["CPlayer_WeaponServices"]
                         ["fields"]["m_hActiveWeapon"],
                   0);
  Offset::WeaponBaseData.Item =
      ReadJsonData(client["client.dll"]["classes"]["C_AttributeContainer"]
                         ["fields"]["m_Item"],
                   0);
  Offset::WeaponBaseData.ItemDefinitionIndex =
      ReadJsonData(client["client.dll"]["classes"]["C_EconItemView"]["fields"]
                         ["m_iItemDefinitionIndex"],
                   0);
  Offset::WeaponBaseData.m_MeshGroupMask = ReadJsonData(
      client["client.dll"]["classes"]["CModelState"]["m_MeshGroupMask"], 0);
  Offset::WeaponBaseData.m_bIsFullAuto =
      ReadJsonData(client["client.dll"]["classes"]["CCSWeaponBaseVData"]
                         ["fields"]["m_bIsFullAuto"],
                   0);

  Offset::EconEntity.AttributeManager = ReadJsonData(
      client["client.dll"]["classes"]["C_EconEntity"]["m_AttributeManager"], 0);
  Offset::EconEntity.FallbackPaintKit = ReadJsonData(
      client["client.dll"]["classes"]["C_EconEntity"]["m_nFallbackPaintKit"],
      0);
  Offset::EconEntity.FallbackSeed = ReadJsonData(
      client["client.dll"]["classes"]["C_EconEntity"]["m_nFallbackSeed"], 0);
  Offset::EconEntity.FallbackWear = ReadJsonData(
      client["client.dll"]["classes"]["C_EconEntity"]["m_flFallbackWear"], 0);
  Offset::EconEntity.FallbackStatTrak = ReadJsonData(
      client["client.dll"]["classes"]["C_EconEntity"]["m_nFallbackStatTrak"],
      0);
  Offset::EconEntity.szCustomName =
      ReadJsonData(client["client.dll"]["classes"]["C_EconItemView"]["fields"]
                         ["m_szCustomName"],
                   0);
  Offset::EconEntity.EntityQuality =
      ReadJsonData(client["client.dll"]["classes"]["C_EconItemView"]["fields"]
                         ["m_iEntityQuality"],
                   0);
  Offset::EconEntity.ItemIDHigh =
      ReadJsonData(client["client.dll"]["classes"]["C_EconItemView"]["fields"]
                         ["m_iItemIDHigh"],
                   0);

  Offset::SmokeGrenadeProjectile.nSmokeEffectTickBegin =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_nSmokeEffectTickBegin"],
                   0);
  Offset::SmokeGrenadeProjectile.bDidSmokeEffect =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_bDidSmokeEffect"],
                   0);
  Offset::SmokeGrenadeProjectile.nRandomSeed =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_nRandomSeed"],
                   0);
  Offset::SmokeGrenadeProjectile.vSmokeColor =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_vSmokeColor"],
                   0);
  Offset::SmokeGrenadeProjectile.vSmokeDetonationPos =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_vSmokeDetonationPos"],
                   0);
  Offset::SmokeGrenadeProjectile.VoxelFrameData =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_VoxelFrameData"],
                   0);
  Offset::SmokeGrenadeProjectile.bSmokeVolumeDataReceived =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_bSmokeVolumeDataReceived"],
                   0);
  Offset::SmokeGrenadeProjectile.bSmokeEffectSpawned =
      ReadJsonData(client["client.dll"]["classes"]["C_SmokeGrenadeProjectile"]
                         ["fields"]["m_bSmokeEffectSpawned"],
                   0);

  return true;
}
