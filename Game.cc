#include "Game.h"

#include "UkiaStuff.h"
bool CGame::InitAddress() {
  this->Address.ClientDLL = reinterpret_cast<DWORD64>(
      Ukia::ProcessMgr.GetProcessModuleHandle("client.dll"));
  this->Address.EngineDLL = reinterpret_cast<DWORD64>(
      Ukia::ProcessMgr.GetProcessModuleHandle("engine2.dll"));
  this->Address.ServerDLL = reinterpret_cast<DWORD64>(
      Ukia::ProcessMgr.GetProcessModuleHandle("server.dll"));
  this->Address.Tier0DLL = reinterpret_cast<DWORD64>(
      Ukia::ProcessMgr.GetProcessModuleHandle("tier0.dll"));
  this->Address.EntityList = GetClientDLLAddress() + Offset::EntityList;
  this->Address.Matrix = GetClientDLLAddress() + Offset::Matrix;
  this->Address.ViewAngle = GetClientDLLAddress() + Offset::ViewAngle;
  this->Address.LocalController =
      GetClientDLLAddress() + Offset::LocalPlayerController;
  this->Address.LocalPawn = GetClientDLLAddress() + Offset::LocalPlayerPawn;
  this->Address.ServerPawn = GetServerDLLAddress() + Offset::LocalPlayerPawn;
  this->Address.PlantedC4PTR = GetClientDLLAddress() + Offset::PlantedC4;
  this->Address.ForceJump = GetClientDLLAddress() + Offset::ForceJump;
  this->Address.ForceCrouch = GetClientDLLAddress() + Offset::ForceCrouch;
  this->Address.ForceForward = GetClientDLLAddress() + Offset::ForceForward;
  this->Address.ForceLeft = GetClientDLLAddress() + Offset::ForceLeft;
  this->Address.ForceRight = GetClientDLLAddress() + Offset::ForceRight;
  this->Address.GlobalVars = GetClientDLLAddress() + Offset::GlobalVars;
  this->Address.BuildNumber = GetEngineDLLAddress() + Offset::BuildNumber;
  
  return this->Address.ClientDLL != 0;
}

DWORD64 CGame::GetClientDLLAddress() { return this->Address.ClientDLL; }

DWORD64 CGame::GetEngineDLLAddress() { return this->Address.EngineDLL; }

DWORD64 CGame::GetServerDLLAddress() { return this->Address.ServerDLL; }

DWORD64 CGame::GetTier0DLLAddress() { return this->Address.Tier0DLL; }

DWORD64 CGame::GetEntityListAddress() { return this->Address.EntityList; }

DWORD64 CGame::GetMatrixAddress() { return this->Address.Matrix; }

DWORD64 CGame::GetViewAngleAddress() { return this->Address.ViewAngle; }

DWORD64 CGame::GetEntityListEntry() { return this->Address.EntityListEntry; }

DWORD64 CGame::GetLocalControllerAddress() {
  return this->Address.LocalController;
}

DWORD64 CGame::GetLocalPawnAddress() { return this->Address.LocalPawn; }

DWORD64 CGame::GetServerPawnAddress() { return this->Address.ServerPawn; }

DWORD64 CGame::GetPlantedC4PTRAddress() { return this->Address.PlantedC4PTR; }

DWORD64 CGame::GetGlobalVarsAddress() {
  DWORD64 Global_Vars_Addr = 0;
  Ukia::ProcessMgr.ReadMemory(this->Address.GlobalVars, Global_Vars_Addr);
  return Global_Vars_Addr;
}
DWORD64 CGame::GetCSGOInputAddress() { return this->Address.CSGOInput; }

DWORD64 CGame::GetBuildNumberAddress() { return this->Address.BuildNumber; }

bool CGame::UpdateEntityListEntry() {
  DWORD64 EntityListEntry = 0;
  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(),
                                            EntityListEntry))
    return false;
  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(EntityListEntry + 0x10,
                                            EntityListEntry))
    return false;

  this->Address.EntityListEntry = EntityListEntry;

  return this->Address.EntityListEntry != 0;
}
bool CGame::GetViewAngles(Vector3& Angle) {

  if (!Ukia::ProcessMgr.ReadMemory<Vector3>(this->Address.ViewAngle, Angle))
    return false;

  return true;
}
bool CGame::SetViewAngles(Vector3 Angle) {

  if (!Ukia::ProcessMgr.WriteMemory<Vector3>(this->Address.ViewAngle, Angle))
    return false;

  return true;
}

bool CGame::SetForceJump(int value) {
  if (!Ukia::ProcessMgr.WriteMemory<int>(this->Address.ForceJump, value))
    return false;

  return true;
}
bool CGame::GetForceJump(int& value) {
  if (!Ukia::ProcessMgr.ReadMemory<int>(this->Address.ForceJump, value))
    return false;

  return true;
}
bool CGame::SetForceCrouch(int value) {
  if (!Ukia::ProcessMgr.WriteMemory<int>(this->Address.ForceCrouch, value))
    return false;

  return true;
}
bool CGame::GetForceCrouch(int& value) {
  if (!Ukia::ProcessMgr.ReadMemory<int>(this->Address.ForceCrouch, value))
    return false;

  return true;
}

// MovingType: 0 = Forward, 1 = Left, 2 = Right
bool CGame::SetForceMove(int MovingType, int Value) {
  switch (MovingType) {
    case 0:
      if (!Ukia::ProcessMgr.WriteMemory<int>(this->Address.ForceForward, Value))
        return false;
      break;
    case 1:
      if (!Ukia::ProcessMgr.WriteMemory<int>(this->Address.ForceLeft, Value))
        return false;
      break;
    case 2:
      if (!Ukia::ProcessMgr.WriteMemory<int>(this->Address.ForceRight, Value))
        return false;
      break;
    default:
      return false;
      break;
  }
  return true;
}

bool CGame::GetForceMove(int MovingType, int& Value) {
  switch (MovingType) {
    case 0:
      if (!Ukia::ProcessMgr.ReadMemory<int>(this->Address.ForceForward, Value))
        return false;
      break;
    case 1:
      if (!Ukia::ProcessMgr.ReadMemory<int>(this->Address.ForceLeft, Value))
        return false;
      break;
    case 2:
      if (!Ukia::ProcessMgr.ReadMemory<int>(this->Address.ForceRight, Value))
        return false;
      break;
    default:
      return false;
      break;
  }
  return true;
}