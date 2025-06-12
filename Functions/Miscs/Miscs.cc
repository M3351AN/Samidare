#include "Miscs.h"

#include <mutex>

namespace Sonar {
struct SoundParams {
  float frequency = 1000.0f;  // 默认频率
  float interval = 1000.0f;   // 默认间隔(ms)
  bool active = false;        // 是否激活
};

std::atomic<bool> running{true};
std::mutex soundMutex;
SoundParams currentParams;

void SoundThread() noexcept {
  auto lastBeep = std::chrono::steady_clock::now();

  while (running) {
    if (!config::Sonar) break;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    SoundParams localParams;
    {
      std::lock_guard<std::mutex> lock(soundMutex);
      localParams = currentParams;
    }

    if (localParams.active) {
      auto now = std::chrono::steady_clock::now();
      auto elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBeep)
              .count();

      if (elapsed >= localParams.interval) {
        Beep(static_cast<DWORD>(localParams.frequency), 35);
        lastBeep = now;
      }
    }
  }
}
}  // namespace Sonar

namespace Misc {

void FoundEnemy(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!config::InfoString) return;

  std::ostringstream allinfo;

  int centerX = static_cast<int>(global::screenSize.x * 0.5f);
  int centerY = static_cast<int>(global::screenSize.y * 0.5f);
  CEntity* best_enemy = nullptr;
  float min_center_dist_sq = FLT_MAX;
  float min_3d_dist = FLT_MAX;
  Vector2 best_screen_pos;
  bool best_on_screen = false;
  const Vector3& local_pos = Vars::LocalEntity.Pawn.CameraPos;
  for (int index = 0; index < ValidEntity.size(); index++) {
    CEntity Entity = ValidEntity[index].first;
    if (!Entity.IsEnemy()) continue;
    if (Entity.Pawn.IsDormanted) continue;
    Vector3 vecPos = Entity.Pawn.Pos;
    Vector3 head_pos = {vecPos.x, vecPos.y, vecPos.z + Entity.Pawn.Height};
    Vector2 head_screen_pos;
    bool on_screen = gGame.View.WorldToScreen(head_pos, head_screen_pos);
    float dist = vecPos.DistanceTo(local_pos);
    float center_dist_sq = FLT_MAX;

    if (on_screen && Entity.ESPAlive()) {
      float dx = head_screen_pos.x - centerX;
      float dy = head_screen_pos.y - centerY;
      center_dist_sq = dx * dx + dy * dy;
    }

    if (center_dist_sq < min_center_dist_sq) {
      min_center_dist_sq = center_dist_sq;
      min_3d_dist = dist;
      best_enemy = &ValidEntity[index].first;
      best_on_screen = on_screen;
      if (on_screen) {
        best_screen_pos = head_screen_pos;
      }
    } else if (center_dist_sq == min_center_dist_sq && dist < min_3d_dist) {
      min_3d_dist = dist;
      best_enemy = &ValidEntity[index].first;
      best_on_screen = on_screen;
      if (on_screen) {
        best_screen_pos = head_screen_pos;
      }
    }
  }

  if (best_enemy) {
    allinfo << "Enemy:\n"
            //<< "Name: " << best_enemy->Controller.PlayerName << "\n" show UTF8
            //incorrect
            << "Address: 0x" << std::hex << best_enemy->Pawn.Address << std::dec
            << "\n"
            << "Health: " << best_enemy->Pawn.Health << " HP\n"
            << "Team: " << best_enemy->Pawn.TeamID << "\n"
            << "Pos: (" << best_enemy->Pawn.Pos.x << ", "
            << best_enemy->Pawn.Pos.y << ", " << best_enemy->Pawn.Pos.z
            << ")\n";

    if (best_on_screen) {
      allinfo << "Screen pos: (" << best_screen_pos.x << ", "
              << best_screen_pos.y << ")\n";
    } else {
      allinfo << "Enemy OOF\n";
    }
  } else {
    allinfo << "No valid enemy\n";
  }

  allinfo << "Local:\n"
          << "Health: " << Vars::LocalEntity.Pawn.Health << " HP\n"
          << "Team: " << Vars::LocalEntity.Pawn.TeamID << "\n"
          << "Pos: (" << local_pos.x << ", " << local_pos.y << ", "
          << local_pos.z << ")\n"
          << "Flags: " << Vars::LocalEntity.Pawn.fFlags << "\n"
          << "FOV: " << Vars::LocalEntity.Pawn.Fov << "\n\n";
  global::infos = allinfo.str();
  return;
}

void SonarRun(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept {
  if (!config::Sonar) return;

  Sonar::SoundParams newParams;
  newParams.active = false;

  int centerX = static_cast<int>(global::screenSize.x * 0.5f);
  int centerY = static_cast<int>(global::screenSize.y * 0.5f);
  CEntity* best_enemy = nullptr;
  float min_center_dist_sq = FLT_MAX;
  float min_3d_dist = FLT_MAX;
  Vector2 best_screen_pos;
  bool best_on_screen = false;
  const Vector3& local_pos = Vars::LocalEntity.Pawn.CameraPos;

  for (int index = 0; index < ValidEntity.size(); index++) {
    CEntity Entity = ValidEntity[index].first;
    if (!Entity.IsEnemy()) continue;
    if (Entity.Pawn.IsDormanted) continue;
    Vector3 vecPos = Entity.Pawn.Pos;
    Vector3 head_pos = {vecPos.x, vecPos.y, vecPos.z + Entity.Pawn.Height};
    Vector2 head_screen_pos;
    bool on_screen = gGame.View.WorldToScreen(head_pos, head_screen_pos);
    float dist = vecPos.DistanceTo(local_pos);
    float center_dist_sq = FLT_MAX;

    if (on_screen) {
      float dx = head_screen_pos.x - centerX;
      float dy = head_screen_pos.y - centerY;
      center_dist_sq = dx * dx + dy * dy;
    }

    if (center_dist_sq < min_center_dist_sq) {
      min_center_dist_sq = center_dist_sq;
      min_3d_dist = dist;
      best_enemy = &ValidEntity[index].first;
      best_on_screen = on_screen;
      if (on_screen) {
        best_screen_pos = head_screen_pos;
      }
    } else if (center_dist_sq == min_center_dist_sq && dist < min_3d_dist) {
      min_3d_dist = dist;
      best_enemy = &ValidEntity[index].first;
      best_on_screen = on_screen;
      if (on_screen) {
        best_screen_pos = head_screen_pos;
      }
    }
  }
  if (best_enemy) {
    if (best_on_screen) {
      float dx = best_screen_pos.x - centerX;
      float dy = best_screen_pos.y - centerY;
      float centerDist = sqrtf(dx * dx + dy * dy);
      float playerDist = min_3d_dist;
      newParams.frequency =
          500 + (3500 * (1 - std::clamp(playerDist / 1000.0f, 0.0f, 1.0f)));
      newParams.interval = 30 + (950 * (centerDist / centerX));
      newParams.active = true;
    }
  }
  {
    std::lock_guard<std::mutex> lock(Sonar::soundMutex);
    Sonar::currentParams = newParams;
  }
  return;
}

void PitchIndicator(CEntity& Local) noexcept {
  if (!config::PitchIndicator) return;
  if (!Vars::IsInGame) return;
  int centerX = static_cast<int>(global::screenSize.x * 0.5f);
  int centerY = static_cast<int>(global::screenSize.y * 0.5f);
  Vector3 ViewAngle;
  if (!gGame.GetViewAngles(ViewAngle)) return;
  float pitch = ViewAngle.x;
  Vector3 camPos = Local.Pawn.CameraPos;
  Vector2 screenPos;
  Vector3 SHITPos = camPos - Vector3{1000.f, 0, 0};
  if (!gGame.View.WorldToScreen(SHITPos, screenPos)) {
    SHITPos = camPos - Vector3{0, 1000.f, 0};
    if (!gGame.View.WorldToScreen(SHITPos, screenPos)) {
      SHITPos = camPos + Vector3{1000.f, 0, 0};
      if (!gGame.View.WorldToScreen(SHITPos, screenPos)) {
        SHITPos = camPos + Vector3{0, 1000.f, 0};
        if (!gGame.View.WorldToScreen(SHITPos, screenPos)) {
          // impossible to get here, but just in case

          float pitchRadians = pitch * (3.14159265f / 180.0f);

          float actualFOV = static_cast<float>(Local.Pawn.Fov);

          float verticalOffsetRatio =
              (pitchRadians / (actualFOV * (3.14159265f / 180.0f)));

          float verticalOffset = verticalOffsetRatio * global::screenSize.y;

          screenPos.y = centerY - static_cast<int>(verticalOffset);
        }
      }
    }
  }

  int dynamicY = static_cast<int>(std::round(screenPos.y));

  RGBA lineColor = {0, 255, 0, 255};
  int lineLength = 15;
  int thickness = 1;
  DrawNewText(centerX - 3, centerY - 3, &lineColor, "^");
  DrawLine(centerX - lineLength, dynamicY, centerX + lineLength - 20, dynamicY,
           &lineColor, thickness);
  DrawLine(centerX - lineLength + 20, dynamicY, centerX + lineLength, dynamicY,
           &lineColor, thickness);
  DrawNewText(centerX + lineLength + 5, dynamicY, &lineColor,
              std::to_string(pitch).c_str());
}
inline void SendKey(WORD vk, bool bKeyDown) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = vk;
  input.ki.dwFlags = bKeyDown ? 0 : KEYEVENTF_KEYUP;

  SendInput(1, &input, sizeof(INPUT));
}
void FastStop(CEntity& Local) noexcept {
  if (!config::FastStop) return;
  const float Trigger_Value = config::FastStopMinVelocity;
  if (!(GetAsyncKeyState('W') || GetAsyncKeyState('A') ||
        GetAsyncKeyState('S') || GetAsyncKeyState('D') ||
        GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_LSHIFT)) &&
      Local.Pawn.Speed > Trigger_Value && Local.Pawn.fFlags != 65664) {
    const auto LocalVel = Local.Pawn.Velocity;
    const auto LocalYaw = Local.Pawn.ViewAngle.y;
    const auto X = (LocalVel.x * cos(LocalYaw / 180 * 3.1415926) +
                    LocalVel.y * sin(LocalYaw / 180 * 3.1415926));
    const auto Y = (LocalVel.y * cos(LocalYaw / 180 * 3.1415926) -
                    LocalVel.x * sin(LocalYaw / 180 * 3.1415926));
    if (X > Trigger_Value) {
      SendKey('S', true);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      SendKey('S', false);
    } else if (X < -Trigger_Value) {
      SendKey('W', true);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      SendKey('W', false);
    }
    if (Y > Trigger_Value) {
      SendKey('D', true);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      SendKey('D', false);
    } else if (Y < -Trigger_Value) {
      SendKey('A', true);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      SendKey('A', false);
    }
  }
}
}  // namespace Misc