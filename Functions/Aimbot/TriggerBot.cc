#include "TriggerBot.h"

#include <XorStr.h>

#include "../../UkiaStuff.h"
#include "../config.h"
#include "mouse_input_injection.h"
#include "../../Driver.h"
namespace TriggerBot {
DWORD uHandle = 0;
DWORD64 ListEntry = 0;
DWORD64 PawnAddress = 0;
CEntity Entity;
bool AllowShoot = false;
bool WaitForNoAttack = false;

inline void ReleaseMouseButton() {
  std::this_thread::sleep_for(std::chrono::milliseconds(config::ShotDuration));
#ifndef USERMODE
  driver.mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#else
  my_mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#endif
}

void TriggerBotRun(const CEntity& LocalEntity) {
  if (!config::TriggerBot) return;
  if (LocalEntity.Controller.AliveStatus == 0) return;
  // When players hold c4/knife/grenades etc., don't shot
  if (LocalEntity.Pawn.MaxAmmo < 1) return;
  Vector3 Aimpunch = LocalEntity.Pawn.AimPunchAngle;
  if (Aimpunch.Length() > config::TriggerMaxRecoil)
    return;
  if (!Ukia::ProcessMgr.ReadMemory<bool>(
          LocalEntity.Pawn.Address + Offset::C_CSPlayerPawn.m_bWaitForNoAttack,
          WaitForNoAttack))
    return;
  if (!Ukia::ProcessMgr.ReadMemory<DWORD>(
          LocalEntity.Pawn.Address + Offset::C_CSPlayerPawnBase.m_iIDEntIndex,
          uHandle))
    return;
  if (uHandle == -1) return;

  ListEntry = Ukia::ProcessMgr.TraceAddress(gGame.GetEntityListAddress(),
                                            {0x8 * (uHandle >> 9) + 0x10, 0x0});
  if (ListEntry == 0) return;

  if (!Ukia::ProcessMgr.ReadMemory<DWORD64>(
          ListEntry + 0x78 * (uHandle & 0x1FF), PawnAddress))
    return;

  if (!Entity.UpdatePawn(PawnAddress)) return;

  if (!config::TriggerIgnoreFlash && LocalEntity.Pawn.FlashDuration > 0.f)
    return;

  if (config::TriggerScopeOnly) {
    if (!LocalEntity.Pawn.Scoped) {
      return;
    }
  }

  AllowShoot = Entity.IsEnemy() && Entity.ESPAlive() &&
               GetBindState(config::TriggerBotHotKey) && !Entity.Pawn.isImmunity;

  if (Entity.Pawn.Pos.DistanceTo(LocalEntity.Pawn.Pos) >= 120.f &&
      LocalEntity.Pawn.WeaponName == XorStr("taser"))
    return;

  if (!AllowShoot) return;

  static std::chrono::time_point LastTimePoint =
      std::chrono::steady_clock::now();
  auto CurTimePoint = std::chrono::steady_clock::now();
  if (CurTimePoint - LastTimePoint >=
      std::chrono::milliseconds(config::TriggerDelay)) {
    const bool isAlreadyShooting = GetAsyncKeyState(VK_LBUTTON) < 0;
    if (!isAlreadyShooting) {
#ifndef USERMODE
      driver.mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
#else
      my_mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
#endif
      std::thread TriggerThread(ReleaseMouseButton);
      TriggerThread.detach();
    }
    LastTimePoint = CurTimePoint;
  }
}
}  // namespace TriggerBot