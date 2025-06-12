#pragma once
#include <Windows.h>
#include <d3d9.h>

#include <iostream>
#include <string>

#include "Imgui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"

#define ABS(X) ((X < 0) ? (-X) : (X))
#define BOX_OFFSET 20.f
#ifndef KEY_H
#define KEY_H
enum EKeyBindMode : int { HOLD = 0, TOGGLE,DISABLE};
struct KeyBind_t {
  constexpr KeyBind_t(int uKey = ImGuiKey_None,
                      int nMode = EKeyBindMode::HOLD)
      : uKey(uKey), nMode(nMode) {}

  bool bEnable = false;
  int uKey = ImGuiKey_None;
  int nMode = EKeyBindMode::HOLD;
};
#endif
#ifndef RGBA_H
#define RGBA_H

typedef struct {
  DWORD R;
  DWORD G;
  DWORD B;
  DWORD A;
} RGBA;
inline RGBA White = {255, 255, 255, 255};
inline RGBA Grey = {155, 155, 155, 255};
inline RGBA Black = {0, 0, 0, 255};
inline RGBA Red = {255, 0, 0, 255};
inline RGBA Green = {0, 255, 0, 255};
inline RGBA Blue = {0, 0, 255, 255};
inline RGBA Transparent = {0, 0, 0, 0};
#endif
#ifndef VECTOR2_H
#define VECTOR2_H
class Vector2 {
 public:
  float x, y;

 public:
  Vector2() : x(0.f), y(0.f) {}
  Vector2(float x_, float y_) : x(x_), y(y_) {}
  Vector2(ImVec2 ImVector2_) : x(ImVector2_.x), y(ImVector2_.y) {}

  Vector2 operator+(Vector2 Vector2_) {
    return {x + Vector2_.x, y + Vector2_.y};
  }
  Vector2 operator-(Vector2 Vector2_) {
    return {x - Vector2_.x, y - Vector2_.y};
  }
  Vector2 operator*(Vector2 Vector2_) {
    return {x * Vector2_.x, y * Vector2_.y};
  }
  Vector2 operator/(Vector2 Vector2_) {
    return {x / Vector2_.x, y / Vector2_.y};
  }
  Vector2 operator*(float n) { return {x / n, y / n}; }
  Vector2 operator/(float n) { return {x / n, y / n}; }
  bool operator==(Vector2 Vector2_) {
    return x == Vector2_.x && y == Vector2_.y;
  }
  bool operator!=(Vector2 Vector2_) {
    return x != Vector2_.x || y != Vector2_.y;
  }
  ImVec2 ToImVector2() noexcept { return ImVec2(x, y); }
  float Length() noexcept { return sqrtf(powf(x, 2) + powf(y, 2)); }
  float DistanceTo(const Vector2& Pos) noexcept {
    return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2));
  }
};
#endif
#ifndef VECTOR3_H
#define VECTOR3_H
class Vector3 {
 public:
  float x, y, z;

 public:
  Vector3() : x(0.f), y(0.f), z(0.f) {}
  Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
  Vector3 operator+(Vector3 Vector3_) {
    return {x + Vector3_.x, y + Vector3_.y, z + Vector3_.z};
  }
  Vector3 operator-(Vector3 Vector3_) {
    return {x - Vector3_.x, y - Vector3_.y, z - Vector3_.z};
  }
  Vector3 operator*(Vector3 Vector3_) {
    return {x * Vector3_.x, y * Vector3_.y, z * Vector3_.z};
  }
  Vector3 operator/(Vector3 Vector3_) {
    return {x / Vector3_.x, y / Vector3_.y, z / Vector3_.z};
  }
  Vector3 operator*(float n) { return {x * n, y * n, z * n}; }
  Vector3 operator/(float n) { return {x / n, y / n, z / n}; }
  bool operator==(Vector3 Vector3_) {
    return x == Vector3_.x && y == Vector3_.y && z == Vector3_.z;
  }
  bool operator!=(Vector3 Vector3_) {
    return x != Vector3_.x || y != Vector3_.y || z != Vector3_.z;
  }
  float Length() noexcept {
    return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
  }
  float DistanceTo(const Vector3& Pos) noexcept {
    return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2) + powf(Pos.z - z, 2));
  }
  void Normalize() noexcept {
    float len = Length();
    if (len != 0) {
      x /= len;
      y /= len;
      z /= len;
    }
  }
  Vector3 Normalized() const {
    Vector3 result = *this;
    result.Normalize();
    return result;
  }
};
#endif
std::string string_To_UTF8(const std::string& str) noexcept;
#ifndef OVERLAY_H
#define OVERLAY_H

void DrawStrokeText(
    float x, float y, RGBA* color, const char* str,
    ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawNewText(
    float x, float y, RGBA* color, const char* str,
                 ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawRect(float x, float y, float w, float h, RGBA* color, float thickness,
              ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawEspBox2D(
    Vector2 feet, Vector2 head, RGBA* color, float thickness,
                  ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawNameTag(Vector2 feet, Vector2 head, char* name,
                 ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawFilledRect(
    float x, float y, float w, float h, RGBA* color,
                    ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawCircleFilled(
    float x, float y, float radius, RGBA* color,
                      ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawCircle(float x, float y, float radius, RGBA* color, float segments,
                ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawTriangle(
    float x1, float y1, float x2, float y2, float x3, float y3, RGBA* color,
                  float thickne, ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawTriangleFilled(
    float x1, float y1, float x2, float y2, float x3, float y3,
                        RGBA* color,
                        ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawLine(float x1, float y1, float x2, float y2, RGBA* color,
              float thickness,
              ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawCornerBox(
    float x, float y, float w, float h, float borderPx, RGBA* color,
                   ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;

void DrawHealthBar(Vector2 feet, Vector2 head, 
                   int curHealth, int maxHealth = 100,
                   RGBA* barColor = &Green,
                   RGBA* backColor = &Transparent,
                   ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) noexcept;
#endif
bool LoadTextureFromMemory(IDirect3DDevice9* device,
                           const unsigned char* image_data, size_t image_size,
                           IDirect3DTexture9** out_texture) noexcept;

bool GetBindState(KeyBind_t& keyBind);
namespace ImGui {
/* @section: main */
bool HotKey(const char* szLabel, int* pKey);
bool HotKey(const char* szLabel, KeyBind_t* pKeyBind,
            const bool bAllowSwitch = true);
}  // namespace ImGui