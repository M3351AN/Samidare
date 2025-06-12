#include "Aimbot.h"

#include <XorStr.h>

#include "../../UkiaStuff.h"
#include "../config.h"
#include "mouse_input_injection.h"
#include "../../Driver.h"

inline void UpdateAngles(const CEntity& Local, Vector3& Angles) {
  auto oldPunch = Vector3{};
  auto shotsFired = Local.Pawn.ShotsFired;

  int ScreenCenterX = global::screenSize.x / 2;
  int ScreenCenterY = global::screenSize.y / 2;
  auto aimPunch = Local.Pawn.AimPunchAngle;
  if (shotsFired) {
    uintptr_t clientState;
    Vector3 viewAngles;
    if (!gGame.GetViewAngles(viewAngles)) return;
    auto newAngles = Vector3{viewAngles.x + oldPunch.x - aimPunch.x * 2.f,
                             viewAngles.y + oldPunch.y - aimPunch.y * 2.f, 0};

    if (newAngles.x > 89.f) newAngles.x = 89.f;

    if (newAngles.x < -89.f) newAngles.x = -89.f;

    while (newAngles.y > 180.f) newAngles.y -= 360.f;

    while (newAngles.y < -180.f) newAngles.y += 360.f;

    newAngles.x += ScreenCenterX;
    newAngles.y += ScreenCenterY;
    Angles = newAngles;
    oldPunch = aimPunch;
  } else {
    oldPunch = aimPunch;
  }

  if (Local.Pawn.Ammo > 1 /*Local.Pawn.ShotsFired > RCSBullet*/) {
    Vector2 PunchAngle;
    if (Local.Pawn.AimPunchCache.Count <= 0 &&
        Local.Pawn.AimPunchCache.Count > 0xFFFF)
      return;
    if (!Ukia::ProcessMgr.ReadMemory<Vector2>(
            Local.Pawn.AimPunchCache.Data +
                (Local.Pawn.AimPunchCache.Count - 1) * sizeof(Vector3),
            PunchAngle))
      return;

    Angles.x = PunchAngle.x;
    Angles.y = PunchAngle.y;
  } else {
    Angles.x = 0.f;
    Angles.y = 0.f;
  }
}

namespace AimBot {

inline void AimBot(const CEntity& Local, Vector3 LocalPos,
                   std::vector<Vector3>& AimPosList) {
  // int isFired;
  // ProcessMgr.ReadMemory(Local.Pawn.Address + Offset::Pawn.iShotsFired,
  // isFired); if (!isFired && !AimLock)
  //  When players hold these weapons, don't aim
  // When players hold c4/knife/grenades etc., don't aim
  if (Local.Pawn.MaxAmmo < 1) {
    Vars::IsAimbotting = false;
    return;
  }
  if (Local.Pawn.ShotsFired <= config::AimBullet && config::AimBullet != 0) {
    Vars::IsAimbotting = false;
    return;
  }
  Vector3 Aimpunch = Local.Pawn.AimPunchAngle;
  if (Aimpunch.Length() > config::TriggerMaxRecoil) {
    Vars::IsAimbotting = false;
    return;
  }
  if (config::AimScopeOnly) {
    if (!Local.Pawn.Scoped) {
      Vars::IsAimbotting = false;
      return;
    }
  }

  if (!config::AimIgnoreFlash && Local.Pawn.FlashDuration > 0.15f) {
    Vars::IsAimbotting = false;
    return;
  }

  int ListSize = AimPosList.size();
  float BestNorm = FLT_MAX;
  if (!ListSize) {
    Vars::IsAimbotting = false;
    return;
  }
  float Yaw, Pitch;
  float Distance, Norm, Length;
  Vector3 Angles{0, 0, 0};
  int ScreenCenterX = global::screenSize.x / 2;
  int ScreenCenterY = global::screenSize.y / 2;
  float TargetX = 0.f;
  float TargetY = 0.f;

  Vector2 ScreenPos;

  bool IsAuto = Local.Pawn.IsAuto;

  for (int i = 0; i < ListSize; i++) {
    Vector3 OppPos;

    OppPos = AimPosList[i] - LocalPos;

    Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

    Length = OppPos.Length();

    // RCS by @Tairitsu
    if (IsAuto) {
      UpdateAngles(Local, Angles);
      float rad = Angles.x / 360.f * M_PI;
      float si = sinf(rad);
      float co = cosf(rad);

      float z = OppPos.z * co + Distance * si;
      float d = (Distance * co - OppPos.z * si) / Distance;

      rad = -Angles.y / 360.f * M_PI;
      si = sinf(rad);
      co = cosf(rad);

      float x = (OppPos.x * co - OppPos.y * si) * d;
      float y = (OppPos.x * si + OppPos.y * co) * d;

      OppPos = Vector3{x, y, z};

      AimPosList[i] = LocalPos + OppPos;
    }

    Yaw = atan2f(OppPos.y, OppPos.x) * 57.2957795131 - Local.Pawn.ViewAngle.y;
    Pitch = -atan(OppPos.z / Distance) * 57.2957795131 - Local.Pawn.ViewAngle.x;
    Norm = sqrt(pow(Yaw, 2) + pow(Pitch, 2));
    if (Norm < BestNorm) BestNorm = Norm;
    gGame.View.WorldToScreen(Vector3(AimPosList[i]), ScreenPos);
  }

  if (Norm < config::AimFov && Norm > config::AimFovMin) {
    // Shake Fixed by @Sweely
    if (ScreenPos.x != ScreenCenterX) {
      TargetX = (ScreenPos.x > ScreenCenterX) ? -(ScreenCenterX - ScreenPos.x)
                                              : ScreenPos.x - ScreenCenterX;
      TargetX /= config::AimSmooth != 0.0f ? config::AimSmooth : 1.5f;
      TargetX = (TargetX + ScreenCenterX > ScreenCenterX * 2 ||
                 TargetX + ScreenCenterX < 0)
                    ? 0
                    : TargetX;
    }

    if (ScreenPos.y != 0) {
      if (ScreenPos.y != ScreenCenterY) {
        TargetY = (ScreenPos.y > ScreenCenterY) ? -(ScreenCenterY - ScreenPos.y)
                                                : ScreenPos.y - ScreenCenterY;
        TargetY /= config::AimSmooth != 0.0f ? config::AimSmooth : 1.5f;
        TargetY = (TargetY + ScreenCenterY > ScreenCenterY * 2 ||
                   TargetY + ScreenCenterY < 0)
                      ? 0
                      : TargetY;
      }
    }

    // Dynamic AimSmooth based on distance
    float DistanceRatio =
        Norm / config::AimFov;  // Calculate the distance ratio
    float SpeedFactor =
        1.0f + (1.0f - DistanceRatio);  // Determine the speed factor based on
                                        // the distance ratio
    TargetX /= (config::AimSmooth * SpeedFactor);
    TargetY /= (config::AimSmooth * SpeedFactor);
    // by Skarbor

    if (ScreenPos.x != ScreenCenterX) {
      TargetX = (ScreenPos.x > ScreenCenterX) ? -(ScreenCenterX - ScreenPos.x)
                                              : ScreenPos.x - ScreenCenterX;
      TargetX /= config::AimSmooth != 0.0f ? config::AimSmooth : 1.5f;
      TargetX = (TargetX + ScreenCenterX > ScreenCenterX * 2 ||
                 TargetX + ScreenCenterX < 0)
                    ? 0
                    : TargetX;
    }

    if (ScreenPos.y != 0) {
      if (ScreenPos.y != ScreenCenterY) {
        TargetY = (ScreenPos.y > ScreenCenterY) ? -(ScreenCenterY - ScreenPos.y)
                                                : ScreenPos.y - ScreenCenterY;
        TargetY /= config::AimSmooth != 0.0f ? config::AimSmooth : 1.5f;
        TargetY = (TargetY + ScreenCenterY > ScreenCenterY * 2 ||
                   TargetY + ScreenCenterY < 0)
                      ? 0
                      : TargetY;
      }
    }
    Vars::IsAimbotting = true;
    #ifndef USERMODE
    driver.mouse_event(MOUSEEVENTF_MOVE, TargetX, TargetY, NULL, NULL);
    #else
    my_mouse_event(MOUSEEVENTF_MOVE, TargetX, TargetY, NULL, NULL);
    #endif
  } else
    Vars::IsAimbotting = false;
}

void AimBotRun(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) {
  if (!config::AimBot) {
    Vars::IsAimbotting = false;
    return;
  }
  // AimBot data
  float DistanceToSight = 0;
  float MaxAimDistance = 100000;
  CEntity NearestEntity;
  Vector3 Angles{0, 0, 0};
  std::vector<Vector3> AimPosList;
  for (int index = 0; index < ValidEntity.size(); index++) {
    CEntity Entity = ValidEntity[index].first;
    if (!Entity.ESPAlive()) continue;
    if (!Entity.IsEnemy()) continue;
    if (!Entity.IsVisible()) continue;

    if (config::HitboxList.size() != 0) {
      for (int p = 0; p < config::HitboxList.size(); p++) {
        Vector3 TempPos;
        if (Entity.Pawn.Address == 0) continue;
        DistanceToSight = Entity.GetBone()
                              .BonePosList[config::HitboxList[p]]
                              .ScreenPos.DistanceTo(
                {global::screenSize.x / 2, global::screenSize.y / 2});

        TempPos = Entity.GetBone().BonePosList[config::HitboxList[p]].Pos;
        /*
        if (LocalEntity.Pawn.ShotsFired >= AimBullet + 1 &&
            MenuConfig::SparyPosition != 0 &&
            NearestEntity.Controller.Address != 0 &&
            Entity.Controller.Address == NearestEntity.Controller.Address) {
          if (HitboxList[p] == MenuConfig::SparyPositionIndex) {
            if (HitboxList[p] == BONEINDEX::head)
              TempPos.z -= 1.f;
            AimPosList.push_back(TempPos);
          }
        } else */
        if (DistanceToSight < MaxAimDistance) {
          MaxAimDistance = DistanceToSight;
          NearestEntity = Entity;
          if (config::HitboxList[p] == BONEINDEX::head) TempPos.z -= 1.f;
          AimPosList.push_back(TempPos);
        }
      }
    }
  }
  if (GetBindState(config::AimBotHotKey)) {
    if (AimPosList.size() != 0) {
      AimBot::AimBot(Vars::LocalEntity, Vars::LocalEntity.Pawn.CameraPos,
                     AimPosList);
    }
  }
}

}  // namespace AimBot