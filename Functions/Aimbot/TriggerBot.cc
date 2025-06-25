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
//bool WaitForNoAttack = false;
HANDLE hDelayQueue, hDurationQueue;

inline void ReleaseMouseButton() {
#ifndef USERMODE
  driver.mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#else
  my_mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#endif
}

inline void DownMouseButton() {
#ifndef USERMODE
  driver.mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
#else
  my_mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
#endif
}

VOID CALLBACK OnDurationEnd(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
  ReleaseMouseButton();
}

VOID CALLBACK OnDelay(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
  DownMouseButton();
  HANDLE hDurationTimer = nullptr;
  CreateTimerQueueTimer(&hDurationTimer, hDurationQueue, OnDurationEnd, nullptr,
                        config::ShotDuration, 0, WT_EXECUTEDEFAULT);
}

void TriggerBotRun(const CEntity& LocalEntity) {

  if (!config::TriggerBot) {
    if (hDelayQueue != 0) {
      DeleteTimerQueueEx(hDelayQueue, nullptr);
      hDelayQueue = 0;
    }
    if (hDurationQueue != 0) {
      DeleteTimerQueueEx(hDurationQueue, nullptr);
      hDurationQueue = 0;
    }
    return;
  }
  if (LocalEntity.Controller.AliveStatus == 0) return;
  // When players hold c4/knife/grenades etc., don't shot
  if (LocalEntity.Pawn.MaxAmmo < 1) return;
  if (hDelayQueue == 0) {
    hDelayQueue = CreateTimerQueue();
  }
  if (hDurationQueue == 0) {
    hDurationQueue = CreateTimerQueue();
  }
  Vector3 Aimpunch = LocalEntity.Pawn.AimPunchAngle;
  if (Aimpunch.Length() > config::TriggerMaxRecoil)
    return;
  /*
  if (!Ukia::ProcessMgr.ReadMemory<bool>(
          LocalEntity.Pawn.Address + Offset::C_CSPlayerPawn.m_bWaitForNoAttack,
          WaitForNoAttack))
    return;
    */
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
  const bool isShootingInDuration =
      (GetAsyncKeyState(VK_LBUTTON) < 0);
  if (!isShootingInDuration) {
    HANDLE hDelayTimer = nullptr;
    CreateTimerQueueTimer(&hDelayTimer, hDelayQueue, OnDelay, nullptr,
                          config::TriggerDelay, 0, WT_EXECUTEDEFAULT);
  }
}
}  // namespace TriggerBot