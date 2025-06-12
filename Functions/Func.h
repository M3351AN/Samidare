#pragma once
#include "../CS2x64.h"

void RenderFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void AimFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void ViewFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void MemoryFunctions(
    std::vector<std::pair<CEntity, DWORD64>>& ValidEntity) noexcept;
void NonMemoryFunctions() noexcept;