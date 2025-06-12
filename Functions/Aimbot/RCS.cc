#include "RCS.h"

#include "mouse_input_injection.h"
#include "../../Driver.h"
namespace RCS {

void RCSRun(CEntity& Local) noexcept {
  if (!config::RCS) return;
  static Vector3 OldPunch;
  if (Local.Pawn.ShotsFired > config::RCSBullet && !Vars::IsAimbotting) {
    Vector3 delta = (OldPunch - (Local.Pawn.AimPunchAngle));

    int MouseX = static_cast<int>(std::round(
        (delta.y * config::RCSScale.x / Vars::Sensitivity) / -0.022f));
    int MouseY = static_cast<int>(std::round(
        (delta.x * config::RCSScale.y / Vars::Sensitivity) / 0.022f));

    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000))
#ifndef USERMODE
      driver.mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
#else
      my_mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
#endif
    OldPunch = Local.Pawn.AimPunchAngle;
  } else {
    OldPunch = Local.Pawn.AimPunchAngle;
  }
}
}  // namespace RCS
