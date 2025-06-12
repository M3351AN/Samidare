#pragma once
#include "../../CS2x64.h"

namespace Sonar {
void SoundThread() noexcept;
}  // namespace Sonar

namespace Misc {
void FoundEnemy(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void SonarRun(std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void PitchIndicator(CEntity& Local) noexcept;
void FastStop(CEntity& Local) noexcept;
}  // namespace Misc