#include "Overlay.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"
#include <XorStr.h>
#include <memory>
#include "Functions/config.h"

std::string string_To_UTF8(const std::string& str) noexcept {
  int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
  wchar_t* pwBuf = new wchar_t[nwLen + 1];
  ZeroMemory(pwBuf, nwLen * 2 + 2);
  ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);
  int nLen =
      ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
  char* pBuf = new char[nLen + 1];
  ZeroMemory(pBuf, nLen + 1);
  ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
  std::string retStr(pBuf);
  delete[] pwBuf;
  delete[] pBuf;
  pwBuf = NULL;
  pBuf = NULL;
  return retStr;
}

void DrawStrokeText(float x, float y, RGBA* color, const char* str,
                    ImDrawList* drawlist) noexcept {
  ImFont a;
  std::string utf_8_1 = std::string(str);
  std::string utf_8_2 = string_To_UTF8(utf_8_1);
  drawlist->AddText(ImVec2(x, y - 1),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)),
                    utf_8_2.c_str());
  drawlist->AddText(ImVec2(x, y + 1),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)),
                    utf_8_2.c_str());
  drawlist->AddText(ImVec2(x - 1, y),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)),
                    utf_8_2.c_str());
  drawlist->AddText(ImVec2(x + 1, y),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)),
                    utf_8_2.c_str());
  drawlist->AddText(ImVec2(x, y),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(color->R / 255.0, color->G / 255.0,
                               color->B / 255.0, color->A / 255.0)),
                    utf_8_2.c_str());
}

void DrawNewText(float x, float y, RGBA* color, const char* str,
                 ImDrawList* drawlist) noexcept {
  ImFont a;
  std::string utf_8_1 = std::string(str);
  std::string utf_8_2 = string_To_UTF8(utf_8_1);
  drawlist->AddText(ImVec2(x, y),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(color->R / 255.0, color->G / 255.0,
                               color->B / 255.0, color->A / 255.0)),
                    utf_8_2.c_str());
}

void DrawRect(float x, float y, float w, float h, RGBA* color, float thickness,
              ImDrawList* drawlist) noexcept {
  drawlist->AddRect(ImVec2(x, y), ImVec2(x + w, y + h),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(color->R / 255.0, color->G / 255.0,
                               color->B / 255.0, color->A / 255.0)),
                    0, 0, thickness);
}

void DrawEspBox2D(Vector2 feet, Vector2 head, RGBA* color, float thickness,
                  ImDrawList* drawlist) noexcept {
  float t = feet.x - head.x;
  float height = feet.y - head.y;
  float pd = feet.x + t;

  ImVec4 boxColor(color->R / 255.0f, color->G / 255.0f, color->B / 255.0f,
                  color->A / 255.0f);

  // Calculate box position and draw the rectangle
  ImVec2 boxMin(head.x - (height / 2) / 2, head.y);
  ImVec2 boxMax(pd + (height / 2) / 2, feet.y);

  drawlist->AddRect(boxMin, boxMax, ImColor(boxColor), 0.0f, 0, thickness);
  drawlist->AddRect(ImVec2(boxMin.x - thickness, boxMin.y - thickness),
                    ImVec2(boxMax.x + thickness, boxMax.y + thickness),
                    ImColor(0, 0, 0, 255), 0.0f, 0, thickness);
  drawlist->AddRect(ImVec2(boxMin.x + thickness, boxMin.y + thickness),
                    ImVec2(boxMax.x - thickness, boxMax.y - thickness),
                    ImColor(0, 0, 0, 255), 0.0f, 0, thickness);
}

void DrawNameTag(Vector2 feet, Vector2 head, char* name,
                 ImDrawList* drawlist) noexcept {
  float t = feet.x - head.x;
  float pd = feet.x + t;

  ImVec2 boxMin(head.x - 5, head.y);
  ImVec2 boxMax(pd, feet.y);

  const ImVec2 textSize = ImGui::CalcTextSize(name);
  const ImVec2 textPos = ImFloor(
      {(boxMin.x + boxMax.x - textSize.x) / 2.f, boxMin.y - textSize.y - 2.f});

  drawlist->AddText(ImVec2(textPos.x + 1.f, textPos.y + 1.f),
                    IM_COL32(0, 0, 0, 255), name);
  drawlist->AddText(textPos, IM_COL32(255, 255, 255, 255), name);
}

void DrawFilledRect(float x, float y, float w, float h, RGBA* color,
                    ImDrawList* drawlist) noexcept {
  drawlist->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
                          ImGui::ColorConvertFloat4ToU32(
                              ImVec4(color->R / 255.0, color->G / 255.0,
                                     color->B / 255.0, color->A / 255.0)),
                          0, 0);
}

void DrawCircleFilled(float x, float y, float radius, RGBA* color,
                      ImDrawList* drawlist) noexcept {
  drawlist->AddCircleFilled(ImVec2(x, y), radius,
                            ImGui::ColorConvertFloat4ToU32(
                                ImVec4(color->R / 255.0, color->G / 255.0,
                                       color->B / 255.0, color->A / 255.0)));
}

void DrawCircle(float x, float y, float radius, RGBA* color, float segments,
                ImDrawList* drawlist) noexcept {
  drawlist->AddCircle(ImVec2(x, y), radius,
                      ImGui::ColorConvertFloat4ToU32(
                          ImVec4(color->R / 255.0, color->G / 255.0,
                                 color->B / 255.0, color->A / 255.0)),
                      segments);
}

void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
                  RGBA* color,
                  float thickne, ImDrawList* drawlist) noexcept {
  drawlist->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
                        ImGui::ColorConvertFloat4ToU32(
                            ImVec4(color->R / 255.0, color->G / 255.0,
                                   color->B / 255.0, color->A / 255.0)),
                        thickne);
}

void DrawTriangleFilled(float x1, float y1, float x2, float y2, float x3,
                        float y3,
                        RGBA* color, ImDrawList* drawlist) noexcept {
  drawlist->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
                              ImGui::ColorConvertFloat4ToU32(
                                  ImVec4(color->R / 255.0, color->G / 255.0,
                                         color->B / 255.0, color->A / 255.0)));
}

void DrawLine(float x1, float y1, float x2, float y2, RGBA* color,
              float thickness,
              ImDrawList* drawlist) noexcept {
  drawlist->AddLine(ImVec2(x1, y1), ImVec2(x2, y2),
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(color->R / 255.0, color->G / 255.0,
                               color->B / 255.0, color->A / 255.0)),
                    thickness);
}

void DrawCornerBox(float x, float y, float w, float h, float borderPx,
                   RGBA* color,
                   ImDrawList* drawlist) noexcept {
  DrawFilledRect(x + borderPx, y, w / 3, borderPx, color, drawlist);
  DrawFilledRect(x + w - w / 3 + borderPx, y, w / 3, borderPx, color, drawlist);
  DrawFilledRect(x, y, borderPx, h / 3, color, drawlist);
  DrawFilledRect(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color,
                 drawlist);
  DrawFilledRect(x + borderPx, y + h + borderPx, w / 3, borderPx, color,
                 drawlist);
  DrawFilledRect(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx,
                 color, drawlist);
  DrawFilledRect(x + w + borderPx, y, borderPx, h / 3, color, drawlist);
  DrawFilledRect(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx,
                 h / 3, color, drawlist);
}
void DrawHealthBar(Vector2 feet, Vector2 head, int curHealth, int maxHealth,
                   RGBA* barColor, RGBA* backColor,
                   ImDrawList* drawlist) noexcept {
  if (curHealth > maxHealth) curHealth = maxHealth;
  if (curHealth < 0) curHealth = 0;
  float boxHeight = feet.y - head.y;
  float t = feet.x - head.x;
  float pd = feet.x + t;
  ImVec2 boxMin(head.x - (boxHeight / 2) / 2, head.y);
  ImVec2 boxMax(pd + (boxHeight / 2) / 2, feet.y);

  int barWidth = 5;
  float barX = boxMin.x - barWidth - 3;
  float barY = boxMin.y;
  float barHeight = boxMax.y - boxMin.y;

  drawlist->AddRectFilled(ImVec2(barX, barY),
                          ImVec2(barX + barWidth, barY + barHeight),
                          ImGui::ColorConvertFloat4ToU32(ImVec4(
                              backColor->R / 255.0f, backColor->G / 255.0f,
                              backColor->B / 255.0f, backColor->A / 255.0f)));

  float healthRatio = (float)curHealth / (float)maxHealth;
  float healthHeight = barHeight * healthRatio;
  float healthTop = barY + (barHeight - healthHeight);

  drawlist->AddRectFilled(ImVec2(barX, healthTop),
                          ImVec2(barX + barWidth, barY + barHeight),
                          ImGui::ColorConvertFloat4ToU32(ImVec4(
                              barColor->R / 255.0f, barColor->G / 255.0f,
                              barColor->B / 255.0f, barColor->A / 255.0f)));

  drawlist->AddRect(ImVec2(barX, barY),
                    ImVec2(barX + barWidth, barY + barHeight),
                    IM_COL32(0, 0, 0, 255));

  if (curHealth < maxHealth) {
    char hpText[16];
    snprintf(hpText, sizeof(hpText), "%d", curHealth);
    ImVec2 textSize = ImGui::CalcTextSize(hpText);
    float textX = barX + (barWidth - textSize.x) / 2.0f;
    float textY = healthTop - textSize.y - 2.0f;
    drawlist->AddText(ImVec2(textX + 1, textY + 1), IM_COL32(0, 0, 0, 255),
                      hpText);
    drawlist->AddText(ImVec2(textX, textY), IM_COL32(255, 255, 255, 255),
                      hpText);
  }
}

bool LoadTextureFromMemory(IDirect3DDevice9* device,
                           const unsigned char* image_data, size_t image_size,
                           IDirect3DTexture9** out_texture) noexcept {
  int width, height, channels;
  unsigned char* data = stbi_load_from_memory(image_data, image_size, &width,
                                              &height, &channels, 4);
  if (!data) return false;

  if (device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8,
                            D3DPOOL_DEFAULT, out_texture, nullptr) != D3D_OK) {
    stbi_image_free(data);
    return false;
  }

  D3DLOCKED_RECT rect;
  (*out_texture)->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
#pragma omp parallel for
  for (int y = 0; y < height; y++) {
#pragma omp parallel for
    for (int x = 0; x < width; x++) {
      unsigned char* src = data + (y * width + x) * 4;
      unsigned char* dest =
          (unsigned char*)rect.pBits + (y * rect.Pitch) + (x * 4);
      dest[0] = src[2];  // R
      dest[1] = src[1];  // G
      dest[2] = src[0];  // B
      dest[3] = src[3];  // A
    }
  }
  (*out_texture)->UnlockRect(0);

  stbi_image_free(data);
  return true;
}

int ImGuiKeyToVK(int imguiKey) {
  switch (imguiKey) {
    case ImGuiKey_Tab:
      return VK_TAB;
    case ImGuiKey_LeftArrow:
      return VK_LEFT;
    case ImGuiKey_RightArrow:
      return VK_RIGHT;
    case ImGuiKey_UpArrow:
      return VK_UP;
    case ImGuiKey_DownArrow:
      return VK_DOWN;
    case ImGuiKey_PageUp:
      return VK_PRIOR;
    case ImGuiKey_PageDown:
      return VK_NEXT;
    case ImGuiKey_Home:
      return VK_HOME;
    case ImGuiKey_End:
      return VK_END;
    case ImGuiKey_Insert:
      return VK_INSERT;
    case ImGuiKey_Delete:
      return VK_DELETE;
    case ImGuiKey_Backspace:
      return VK_BACK;
    case ImGuiKey_Space:
      return VK_SPACE;
    case ImGuiKey_Enter:
      return VK_RETURN;
    case ImGuiKey_Escape:
      return VK_ESCAPE;
    case ImGuiKey_A:
      return 'A';
    case ImGuiKey_B:
      return 'B';
    case ImGuiKey_C:
      return 'C';
    case ImGuiKey_D:
      return 'D';
    case ImGuiKey_E:
      return 'E';
    case ImGuiKey_F:
      return 'F';
    case ImGuiKey_G:
      return 'G';
    case ImGuiKey_H:
      return 'H';
    case ImGuiKey_I:
      return 'I';
    case ImGuiKey_J:
      return 'J';
    case ImGuiKey_K:
      return 'K';
    case ImGuiKey_L:
      return 'L';
    case ImGuiKey_M:
      return 'M';
    case ImGuiKey_N:
      return 'N';
    case ImGuiKey_O:
      return 'O';
    case ImGuiKey_P:
      return 'P';
    case ImGuiKey_Q:
      return 'Q';
    case ImGuiKey_R:
      return 'R';
    case ImGuiKey_S:
      return 'S';
    case ImGuiKey_T:
      return 'T';
    case ImGuiKey_U:
      return 'U';
    case ImGuiKey_V:
      return 'V';
    case ImGuiKey_W:
      return 'W';
    case ImGuiKey_X:
      return 'X';
    case ImGuiKey_Y:
      return 'Y';
    case ImGuiKey_Z:
      return 'Z';
    case ImGuiKey_F1:
      return VK_F1;
    case ImGuiKey_F2:
      return VK_F2;
    case ImGuiKey_F3:
      return VK_F3;
    case ImGuiKey_F4:
      return VK_F4;
    case ImGuiKey_F5:
      return VK_F5;
    case ImGuiKey_F6:
      return VK_F6;
    case ImGuiKey_F7:
      return VK_F7;
    case ImGuiKey_F8:
      return VK_F8;
    case ImGuiKey_F9:
      return VK_F9;
    case ImGuiKey_F10:
      return VK_F10;
    case ImGuiKey_F11:
      return VK_F11;
    case ImGuiKey_F12:
      return VK_F12;
    case ImGuiKey_MouseLeft:
      return VK_LBUTTON;
    case ImGuiKey_MouseRight:
      return VK_RBUTTON;
    case ImGuiKey_MouseMiddle:
      return VK_MBUTTON;
    case ImGuiKey_MouseX1:
      return VK_XBUTTON1;
    case ImGuiKey_MouseX2:
      return VK_XBUTTON2;

    case ImGuiKey_0:
      return '0';
    case ImGuiKey_1:
      return '1';
    case ImGuiKey_2:
      return '2';
    case ImGuiKey_3:
      return '3';
    case ImGuiKey_4:
      return '4';
    case ImGuiKey_5:
      return '5';
    case ImGuiKey_6:
      return '6';
    case ImGuiKey_7:
      return '7';
    case ImGuiKey_8:
      return '8';
    case ImGuiKey_9:
      return '9';

    case ImGuiKey_Keypad0:
      return VK_NUMPAD0;
    case ImGuiKey_Keypad1:
      return VK_NUMPAD1;
    case ImGuiKey_Keypad2:
      return VK_NUMPAD2;
    case ImGuiKey_Keypad3:
      return VK_NUMPAD3;
    case ImGuiKey_Keypad4:
      return VK_NUMPAD4;
    case ImGuiKey_Keypad5:
      return VK_NUMPAD5;
    case ImGuiKey_Keypad6:
      return VK_NUMPAD6;
    case ImGuiKey_Keypad7:
      return VK_NUMPAD7;
    case ImGuiKey_Keypad8:
      return VK_NUMPAD8;
    case ImGuiKey_Keypad9:
      return VK_NUMPAD9;
    case ImGuiKey_KeypadDecimal:
      return VK_DECIMAL;
    case ImGuiKey_KeypadDivide:
      return VK_DIVIDE;
    case ImGuiKey_KeypadMultiply:
      return VK_MULTIPLY;
    case ImGuiKey_KeypadSubtract:
      return VK_SUBTRACT;
    case ImGuiKey_KeypadAdd:
      return VK_ADD;
    case ImGuiKey_KeypadEnter:
      return VK_RETURN;
    case ImGuiKey_KeypadEqual:
      return VK_OEM_PLUS;

    case ImGuiKey_Apostrophe:
      return VK_OEM_7;  // '
    case ImGuiKey_Comma:
      return VK_OEM_COMMA;  // ,
    case ImGuiKey_Minus:
      return VK_OEM_MINUS;  // -
    case ImGuiKey_Period:
      return VK_OEM_PERIOD;  // .
    case ImGuiKey_Slash:
      return VK_OEM_2;  // /
    case ImGuiKey_Semicolon:
      return VK_OEM_1;  // ;
    case ImGuiKey_Equal:
      return VK_OEM_PLUS;  // =
    case ImGuiKey_LeftBracket:
      return VK_OEM_4;  // [
    case ImGuiKey_Backslash:
      return VK_OEM_5;  // \

    case ImGuiKey_RightBracket: return VK_OEM_6;     // ]
    case ImGuiKey_GraveAccent:
      return VK_OEM_3;  // `

    case ImGuiKey_LeftCtrl:
      return VK_LCONTROL;
    case ImGuiKey_RightCtrl:
      return VK_RCONTROL;
    case ImGuiKey_LeftShift:
      return VK_LSHIFT;
    case ImGuiKey_RightShift:
      return VK_RSHIFT;
    case ImGuiKey_LeftAlt:
      return VK_LMENU;
    case ImGuiKey_RightAlt:
      return VK_RMENU;
    case ImGuiKey_LeftSuper:
      return VK_LWIN;
    case ImGuiKey_RightSuper:
      return VK_RWIN;

    case ImGuiKey_CapsLock:
      return VK_CAPITAL;
    case ImGuiKey_ScrollLock:
      return VK_SCROLL;
    case ImGuiKey_NumLock:
      return VK_NUMLOCK;

    case ImGuiKey_PrintScreen:
      return VK_SNAPSHOT;
    case ImGuiKey_Pause:
      return VK_PAUSE;
    case ImGuiKey_Menu:
      return VK_APPS;

    case ImGuiKey_F13:
      return VK_F13;
    case ImGuiKey_F14:
      return VK_F14;
    case ImGuiKey_F15:
      return VK_F15;
    case ImGuiKey_F16:
      return VK_F16;
    case ImGuiKey_F17:
      return VK_F17;
    case ImGuiKey_F18:
      return VK_F18;
    case ImGuiKey_F19:
      return VK_F19;
    case ImGuiKey_F20:
      return VK_F20;
    case ImGuiKey_F21:
      return VK_F21;
    case ImGuiKey_F22:
      return VK_F22;
    case ImGuiKey_F23:
      return VK_F23;
    case ImGuiKey_F24:
      return VK_F24;

    case ImGuiKey_AppBack:
      return VK_BROWSER_BACK;
    case ImGuiKey_AppForward:
      return VK_BROWSER_FORWARD;

    default:
      return 0; 
  }
}

bool GetBindState(KeyBind_t& keyBind) {
  if (keyBind.uKey == ImGuiKey_None) return false;

  int vk = ImGuiKeyToVK(keyBind.uKey);
  if (vk == 0) return false;

  switch (keyBind.nMode) {
    case EKeyBindMode::HOLD:
      keyBind.bEnable = (GetAsyncKeyState(vk) & 0x8000) != 0;
      break;
    case EKeyBindMode::TOGGLE: {
      static bool lastDown[256] = {0};
      bool isDown = (GetAsyncKeyState(vk) & 0x8000) != 0;
      if (isDown && !lastDown[vk]) {
        keyBind.bEnable = !keyBind.bEnable;
      }
      lastDown[vk] = isDown;
      break;
    }
    case EKeyBindMode::DISABLE: 
      keyBind.bEnable = (GetAsyncKeyState(vk) & 0x8000) == 0;
      break;
  }
  return keyBind.bEnable;
}

bool ImGui::HotKey(const char* szLabel, int* pKey) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* pWindow = g.CurrentWindow;
    if (pWindow->SkipItems) return false;

    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;
    const ImGuiID nIndex = pWindow->GetID(szLabel);

    const float flWidth = CalcItemWidth();
    const ImVec2 vecLabelSize = CalcTextSize(szLabel, nullptr, true);

    const ImRect rectFrame(
        pWindow->DC.CursorPos +
            ImVec2(vecLabelSize.x > 0.0f
                       ? style.ItemInnerSpacing.x + GetFrameHeight()
                       : 0.0f,
                   0.0f),
        pWindow->DC.CursorPos +
            ImVec2(flWidth, vecLabelSize.x > 0.0f
                                ? vecLabelSize.y + style.FramePadding.y
                                : 0.f));
    const ImRect rectTotal(rectFrame.Min, rectFrame.Max);

    ItemSize(rectTotal, style.FramePadding.y);
    if (!ItemAdd(rectTotal, nIndex, &rectFrame)) return false;

    const bool bHovered = ItemHoverable(rectFrame, nIndex, NULL);
    if (bHovered) {
        SetHoveredID(nIndex);
        g.MouseCursor = ImGuiMouseCursor_TextInput;
    }

    const bool bClicked = bHovered && io.MouseClicked[0];
    const bool bDoubleClicked = bHovered && io.MouseDoubleClicked[0];
    static ImGuiID lastActiveId = 0;
    static int skipFrame = 0;

    if (bClicked || bDoubleClicked) {
        if (g.ActiveId != nIndex) {
            std::memset(io.MouseDown, 0, sizeof(io.MouseDown));
            io.ClearInputKeys();
            *pKey = ImGuiKey_None;
            skipFrame = 1; // 激活后跳过一帧输入
            lastActiveId = nIndex;
        }
        SetActiveID(nIndex, pWindow);
        FocusWindow(pWindow);
    }

    bool bValueChanged = false;
    if (int nKey = *pKey; g.ActiveId == nIndex) {
        if (skipFrame > 0 && lastActiveId == nIndex) {
            --skipFrame;
        } else {
            for (int n = ImGuiKey_NamedKey_BEGIN; n < ImGuiKey_NamedKey_END; ++n) {
                if (ImGui::IsKeyPressed((ImGuiKey)n, false)) {
                    nKey = (ImGuiKey)n;
                    bValueChanged = true;
                    ClearActiveID();
                    break;
                }
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                *pKey = ImGuiKey_None;
                ClearActiveID();
            } else {
                *pKey = nKey;
            }
        }
    }

    char szBuffer[64] = {};
    char* szBufferEnd = std::strcpy(szBuffer, "  ");
    if (*pKey != ImGuiKey_None && g.ActiveId != nIndex)
        szBufferEnd = std::strcat(szBufferEnd, ImGui::GetKeyName(static_cast<ImGuiKey>(*pKey)));
    else if (g.ActiveId == nIndex)
      szBufferEnd =
          std::strcat(szBufferEnd, LangSettings::TextPress.c_str());
    else
      szBufferEnd =
          std::strcat(szBufferEnd, LangSettings::TextNone.c_str());
    std::strcat(szBufferEnd, "  ");

    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
    const ImVec2 vecBufferSize = CalcTextSize(szBuffer);
    RenderFrame(ImVec2(rectFrame.Max.x - vecBufferSize.x, rectTotal.Min.y),
                ImVec2(rectFrame.Max.x,
                       rectTotal.Min.y + style.FramePadding.y + vecBufferSize.y),
                GetColorU32((bHovered || bClicked || bDoubleClicked)
                                ? ImGuiCol_FrameBgHovered
                                : ImGuiCol_FrameBg),
                true, style.FrameRounding);
    pWindow->DrawList->AddText(
        ImVec2(rectFrame.Max.x - vecBufferSize.x,
               rectTotal.Min.y + style.FramePadding.y),
        GetColorU32(g.ActiveId == nIndex ? ImGuiCol_Text : ImGuiCol_TextDisabled),
        szBuffer);

    if (vecLabelSize.x > 0.f)
        RenderText(ImVec2(rectTotal.Min.x, rectTotal.Min.y + style.FramePadding.y),
                   szLabel);

    PopStyleVar();
    return bValueChanged;
}

bool ImGui::HotKey(const char* szLabel, KeyBind_t* pKeyBind,
                   const bool bAllowSwitch) {
  const bool bValueChanged = HotKey(szLabel, &pKeyBind->uKey);

  if (bAllowSwitch) {
    PushID(szLabel);
    if (IsItemClicked(ImGuiMouseButton_Right)) OpenPopup(XorStr("key##Popup"));

    if (BeginPopup(XorStr("key##Popup"))) {
      SetNextItemWidth(95.f);
      const std::string& hold = LangSettings::TextHold;
      const std::string& toggle = LangSettings::TextToggle;
      const std::string& disable = LangSettings::TextDisable;
      std::string comboItems = hold + '\0' + toggle + '\0' + disable + '\0';
      comboItems.push_back('\0');

      if (Combo(XorStr("##keybind.mode"),
                reinterpret_cast<int*>(&pKeyBind->nMode), comboItems.c_str()))
        CloseCurrentPopup();

      EndPopup();
    }

    PopID();
  }

  return bValueChanged;
}