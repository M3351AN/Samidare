#include <ShlObj.h>
#include <mmsystem.h>
#include <stdio.h>

#include <array>
#include <cmath>
#include <execution>
#include <filesystem>
#include <limits>
#include <thread>
#include <unordered_map>

#include "Functions/Func.h"
#include "Game.h"
#include "Lang.h"
#include "UkiaStuff.h"
#include "uiaccess.h"
#include "shigure.h"
#include "zekamashi.h"
#pragma comment(lib, "winmm.lib")

enum ZBID {
  ZBID_DEFAULT = 0,
  ZBID_DESKTOP = 1,
  ZBID_UIACCESS = 2,
  ZBID_IMMERSIVE_IHM = 3,
  ZBID_IMMERSIVE_NOTIFICATION = 4,
  ZBID_IMMERSIVE_APPCHROME = 5,
  ZBID_IMMERSIVE_MOGO = 6,
  ZBID_IMMERSIVE_EDGY = 7,
  ZBID_IMMERSIVE_INACTIVEMOBODY = 8,
  ZBID_IMMERSIVE_INACTIVEDOCK = 9,
  ZBID_IMMERSIVE_ACTIVEMOBODY = 10,
  ZBID_IMMERSIVE_ACTIVEDOCK = 11,
  ZBID_IMMERSIVE_BACKGROUND = 12,
  ZBID_IMMERSIVE_SEARCH = 13,
  ZBID_GENUINE_WINDOWS = 14,
  ZBID_IMMERSIVE_RESTRICTED = 15,
  ZBID_SYSTEM_TOOLS = 16,
  // Win10
  ZBID_LOCK = 17,
  ZBID_ABOVELOCK_UX = 18,
};

typedef HWND(WINAPI* CreateWindowInBand)(
    _In_ DWORD dwExStyle, _In_opt_ ATOM atom, _In_opt_ LPCWSTR lpWindowName,
    _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth,
    _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam, DWORD band);

CreateWindowInBand pCreateWindowInBand =
    reinterpret_cast<CreateWindowInBand>(GetProcAddress(
        LoadLibraryA(XorStr("user32.dll")), XorStr("CreateWindowInBand")));

void JustGetWindowRect() {
  if (Ukia::IsFullscreen(global::hwnd_)) {
    global::screenSize.x = GetSystemMetrics(SM_CXSCREEN);
    global::screenSize.y = GetSystemMetrics(SM_CYSCREEN);
    global::screenPos.x = 0;
    global::screenPos.y = 0;
  } else {
    RECT clientRect;
    if (GetClientRect(global::hwnd_, &clientRect)) {
      int clientWidth = clientRect.right - clientRect.left;
      int clientHeight = clientRect.bottom - clientRect.top;

      global::screenSize.x = clientWidth;
      global::screenSize.y = clientHeight;
    } else {
      MessageBoxA(nullptr, XorStr("Failed to get window rect."),
                  XorStr("Samidare"), MB_OK);
      Ukia::UkiaExit();
    }
  }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam,
                         LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
    return true;

  switch (Message) {
    case WM_DESTROY:
      if (DirectX9Interface::pDevice != NULL) {
        DirectX9Interface::pDevice->EndScene();
        DirectX9Interface::pDevice->Release();
      }
      if (DirectX9Interface::Direct3D9 != NULL) {
        DirectX9Interface::Direct3D9->Release();
      }
      PostQuitMessage(0);
      Ukia::UkiaExit(4);
      break;
    case WM_MOVE:
    case WM_SIZE:
      if (DirectX9Interface::pDevice != NULL && wParam != SIZE_MINIMIZED) {
        if (LOWORD(lParam) > 0 && HIWORD(lParam) > 0) {
          ImGui_ImplDX9_InvalidateDeviceObjects();
          DirectX9Interface::pParams.BackBufferWidth = LOWORD(lParam);
          DirectX9Interface::pParams.BackBufferHeight = HIWORD(lParam);
        }
      }
      break;
    default:
      return DefWindowProc(hWnd, Message, wParam, lParam);
      break;
  }
  return 0;
}

std::mutex g_d3dMutex;
struct WindowStateTracker {
  RECT oldRect = {0};
  bool lastMenuState = false;
  bool wasGameFocused = true;
  bool lastFullscreen = false;

  template <typename T>
  void UpdateWindowState(T&& checker) {
    lastFullscreen = checker();
  }
};

bool HandleFocusState(bool& wasFocused) {
  const HWND foreground = GetForegroundWindow();
  const bool focused =
      (foreground == global::hwnd_) || (foreground == OverlayWindow::Hwnd);

  if (focused != wasFocused) {
    ShowWindow(OverlayWindow::Hwnd, focused ? SW_SHOW : SW_HIDE);
    if (focused) {
      SetWindowPos(OverlayWindow::Hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    wasFocused = focused;
  }
  return focused;
}

void SyncMenuState(bool& lastState) {
  if (config::ShowMenu == lastState) return;

  DWORD newExStyle = WS_EX_TOPMOST | WS_EX_LAYERED;
  newExStyle |= config::ShowMenu ? 0 : WS_EX_TRANSPARENT;

  SetWindowLongPtr(OverlayWindow::Hwnd, GWL_EXSTYLE, newExStyle);

  SetWindowPos(OverlayWindow::Hwnd, HWND_TOPMOST, 0, 0, 0, 0,
               SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

  if (!config::ShowMenu) {
    SetForegroundWindow(global::hwnd_);
    SetActiveWindow(global::hwnd_);
    SetFocus(global::hwnd_);
  }

  lastState = config::ShowMenu;
}

void ProcessMessageQueue() {
  MSG msg;
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void SyncOverlayPosition(WindowStateTracker& stateTracker) {
  std::lock_guard<std::mutex> lock(g_d3dMutex);
  RECT clientRect;
  GetClientRect(global::hwnd_, &clientRect);
  POINT clientPos{0};
  ClientToScreen(global::hwnd_, &clientPos);

  bool positionChanged = (clientPos.x != stateTracker.oldRect.left) ||
                         (clientPos.y != stateTracker.oldRect.top);
  bool sizeChanged =
      (clientRect.right - clientRect.left != stateTracker.oldRect.right) ||
      (clientRect.bottom - clientRect.top != stateTracker.oldRect.bottom);

  if (positionChanged || sizeChanged) {
    SetWindowPos(OverlayWindow::Hwnd, HWND_TOPMOST, clientPos.x, clientPos.y,
                 clientRect.right, clientRect.bottom,
                 SWP_NOZORDER | SWP_NOACTIVATE);

    global::screenSize.x = clientRect.right;
    global::screenSize.y = clientRect.bottom;
    stateTracker.oldRect = {0, 0, clientRect.right, clientRect.bottom};

    global::screenSize.x = clientRect.right;
    global::screenSize.y = clientRect.bottom;

    DirectX9Interface::pParams.BackBufferWidth = global::screenSize.x;
    DirectX9Interface::pParams.BackBufferHeight = global::screenSize.y;
  }
}

void UpdateInputState() noexcept {
  ImGuiIO& io = ImGui::GetIO();

  POINT cursorPos{0};
  GetCursorPos(&cursorPos);
  ScreenToClient(global::hwnd_, &cursorPos);
  io.MousePos =
      ImVec2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
  io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

  static std::unordered_map<UINT, bool> keyStates;

  bool insertState = (GetAsyncKeyState(VK_INSERT) & 0x8000);
  bool deleteState = (GetAsyncKeyState(VK_DELETE) & 0x8000);

  if ((insertState || deleteState) && !keyStates[VK_INSERT] &&
      !keyStates[VK_DELETE]) {
    config::ShowMenu = !config::ShowMenu;
  }

  keyStates[VK_INSERT] = insertState;
  keyStates[VK_DELETE] = deleteState;

  io.KeyCtrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
  io.KeyShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
  io.KeyAlt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
}

void HandlePresentResult(HRESULT result) {
  if (result == D3DERR_DEVICELOST &&
      DirectX9Interface::pDevice->TestCooperativeLevel() ==
          D3DERR_DEVICENOTRESET) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
    ImGui_ImplDX9_CreateDeviceObjects();
  }
}

void RenderFrame() noexcept {
  std::lock_guard<std::mutex> lock(g_d3dMutex);
  ImGui_ImplDX9_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  RenderFunctions(Vars::ValidEntity);

  ImGui::EndFrame();

  if (SUCCEEDED(DirectX9Interface::pDevice->BeginScene())) {
    DirectX9Interface::pDevice->Clear(0, NULL, D3DCLEAR_TARGET,
                                      D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    DirectX9Interface::pDevice->EndScene();
  }

  HandlePresentResult(
      DirectX9Interface::pDevice->Present(NULL, NULL, NULL, NULL));
}

void CleanupRenderResources() {
  ImGui_ImplDX9_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  if (DirectX9Interface::pDevice) {
    DirectX9Interface::pDevice->Release();
    DirectX9Interface::pDevice = nullptr;
  }

  if (OverlayWindow::Hwnd) {
    DestroyWindow(OverlayWindow::Hwnd);
    UnregisterClassA(OverlayWindow::WindowClass.lpszClassName,
                     OverlayWindow::WindowClass.hInstance);
  }
}

void MainLoop() noexcept {
  static WindowStateTracker stateTracker;
  MSG& msg = DirectX9Interface::Message;
  ZeroMemory(&msg, sizeof(MSG));

  while (msg.message != WM_QUIT) {
    if (!global::isRunning) break;

    if (config::BypassCapture)
      SetWindowDisplayAffinity(OverlayWindow::Hwnd, WDA_EXCLUDEFROMCAPTURE);
    else
      SetWindowDisplayAffinity(OverlayWindow::Hwnd, WDA_NONE);

    stateTracker.UpdateWindowState([&]() {
      const bool isFullscreen = Ukia::IsFullscreen(global::hwnd_);
      if (isFullscreen != stateTracker.lastFullscreen) {
        JustGetWindowRect();
        return true;
      }
      return false;
    });

    global::isFocused = HandleFocusState(stateTracker.wasGameFocused);
    if (!global::isFocused) {
      std::this_thread::sleep_for(std::chrono::milliseconds(15));
      continue;
    }

    SyncMenuState(stateTracker.lastMenuState);

    ProcessMessageQueue();

    SyncOverlayPosition(stateTracker);

    UpdateInputState();

    if (global::fontUpdatePending) font_manager.ReloadFonts();

    RenderFrame();
    std::this_thread::sleep_for(
        std::chrono::milliseconds(Vars::RenderInterval));
  }

  CleanupRenderResources();
}

static void* MySettingsReadOpen(ImGuiContext*, ImGuiSettingsHandler*,
                                const char* name) {
  return (strcmp(name, "Language") == 0) ? (void*)1 : nullptr;
}

static void MySettingsReadLine(ImGuiContext*, ImGuiSettingsHandler*, void*,
                               const char* line) {
  char buf[256] = {};
  if (sscanf(line, "Selected=%255s", buf) == 1 && buf[0] != '\0') {
    MyConfigSaver::selectedLangsFile = buf;
    MyConfigSaver::LoadLangs(MyConfigSaver::selectedLangsFile + ".yaml");
  }
}

void AfterImGuiIniLoaded() {
  if (MyConfigSaver::selectedLangsFile.empty()) {
    MyConfigSaver::selectedLangsFile = "Default";
    MyConfigSaver::LoadLangs("Default.yaml");
  }
}

static void MySettingsWriteAll(ImGuiContext*, ImGuiSettingsHandler* handler,
                               ImGuiTextBuffer* out_buf) {
  out_buf->appendf("[%s][Language]\nSelected=%s\n", handler->TypeName,
                   MyConfigSaver::selectedLangsFile.c_str());
}

void RegisterImGuiLangSettingsHandler() {
  ImGuiSettingsHandler ini_handler;
  ini_handler.TypeName = "Samidare";
  ini_handler.TypeHash = ImHashStr("Samidare");
  ini_handler.ReadOpenFn = MySettingsReadOpen;
  ini_handler.ReadLineFn = MySettingsReadLine;
  ini_handler.WriteAllFn = MySettingsWriteAll;
  ImGui::GetCurrentContext()->SettingsHandlers.push_back(ini_handler);
}

bool DirectXInit() noexcept {
  if (FAILED(
          Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::Direct3D9))) {
    return false;
  }

  D3DPRESENT_PARAMETERS Params = {0};
  Params.Windowed = TRUE;
  Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
  Params.hDeviceWindow = OverlayWindow::Hwnd;
  Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
  Params.BackBufferFormat = D3DFMT_A8R8G8B8;
  Params.BackBufferWidth = global::screenSize.x;
  Params.BackBufferHeight = global::screenSize.y;
  Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
  Params.EnableAutoDepthStencil = TRUE;
  Params.AutoDepthStencilFormat = D3DFMT_D16;
  Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
  Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

  if (FAILED(DirectX9Interface::Direct3D9->CreateDeviceEx(
          D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayWindow::Hwnd,
          D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0,
          &DirectX9Interface::pDevice))) {
    DirectX9Interface::Direct3D9->Release();
    return false;
  }

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  static std::string iniPath;
  iniPath = config::path + XorStr("\\imgui_config.ini");
  io.IniFilename = iniPath.c_str();
  MyConfigSaver::ExportDefaultLang();
  RegisterImGuiLangSettingsHandler();
  AfterImGuiIniLoaded();
  ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput ||
      ImGui::GetIO().WantCaptureKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImFontConfig font_cfg;
  font_cfg.FontDataOwnedByAtlas = false;
  static constexpr ImWchar basic_ranges[] = {

      0x0020, 0x00FF,  // ASCII + Latin-1 Supplement
      0x0102, 0x0103, 0x0110, 0x0111, 0x0128, 0x0129, 0x0168,
      0x0169, 0x01A0, 0x01A1, 0x01AF, 0x01B0,

      0x0400, 0x052F, 0x2DE0, 0x2DFF, 0xA640, 0xA69F,

      0x2000, 0x206F, 0xFF00, 0xFFEF, 0xFFFD, 0xFFFD, 0};
  ImFont* font = io.Fonts->AddFontFromMemoryTTF(
      (void*)harmonySans, sizeof(harmonySans), 16.0f, &font_cfg, basic_ranges);

  io.Fonts->Build();

  // io.FontDefault = font;

  ImGui_ImplWin32_EnableDpiAwareness();
  ImGui_ImplWin32_Init(OverlayWindow::Hwnd);
  ImGui_ImplDX9_Init(DirectX9Interface::pDevice);
  LoadTextureFromMemory(DirectX9Interface::pDevice, ShigureImg,
                        sizeof(ShigureImg), &global::Shigure);
  LoadTextureFromMemory(DirectX9Interface::pDevice, ZekamashiImg,
                        sizeof(ZekamashiImg), &global::Zekamashi);
  DirectX9Interface::Direct3D9->Release();
  return true;
}
static LPCSTR randomWindowName;
static LPCWSTR randomWindowNameW;
void SetupWindow() noexcept {
  if (global::hwnd_) {
    static RECT TempRect = {NULL};
    static POINT TempPoint;
    GetClientRect(global::hwnd_, &TempRect);
    ClientToScreen(global::hwnd_, &TempPoint);
    TempRect.left = TempPoint.x;
    TempRect.top = TempPoint.y;
    global::screenSize.x = TempRect.right;
    global::screenSize.y = TempRect.bottom;
  }

  JustGetWindowRect();  // again.

  if (global::uiAccessStatus != ERROR_SUCCESS) {
    OverlayWindow::WindowClass = {};
    OverlayWindow::WindowClass.cbSize = sizeof(WNDCLASSEX);
    OverlayWindow::WindowClass.style = 0;
    OverlayWindow::WindowClass.lpfnWndProc = WinProc;
    OverlayWindow::WindowClass.cbClsExtra = 0;
    OverlayWindow::WindowClass.cbWndExtra = 0;
    OverlayWindow::WindowClass.hInstance = GetModuleHandle(NULL);
    OverlayWindow::WindowClass.hIcon = NULL;
    OverlayWindow::WindowClass.hCursor = NULL;
    OverlayWindow::WindowClass.hbrBackground = nullptr;
    OverlayWindow::WindowClass.lpszMenuName = nullptr;
    OverlayWindow::WindowClass.lpszClassName = OverlayWindow::Name;
    OverlayWindow::WindowClass.hIconSm = NULL;

    RegisterClassExA(&OverlayWindow::WindowClass);
    OverlayWindow::Hwnd = CreateWindowExA(
        WS_EX_TOPMOST, OverlayWindow::Name, OverlayWindow::Name,
        WS_POPUP | WS_VISIBLE, global::screenPos.x, global::screenPos.y,
        global::screenSize.x, global::screenSize.y, NULL, NULL,
        OverlayWindow::WindowClass.hInstance, NULL);
  } else {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WinProc;
    wc.cbClsExtra = 0L;
    wc.cbWndExtra = 0L;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = randomWindowNameW;
    wc.hIconSm = NULL;
    auto res = RegisterClassExW(&wc);
    OverlayWindow::Hwnd = pCreateWindowInBand(
        WS_EX_TOPMOST, res, randomWindowNameW, WS_POPUP | WS_VISIBLE,
        global::screenPos.x, global::screenPos.y, global::screenSize.x,
        global::screenSize.y, NULL, NULL, wc.hInstance, NULL, ZBID_UIACCESS);
  }
  if (OverlayWindow::Hwnd != NULL)
    DwmExtendFrameIntoClientArea(OverlayWindow::Hwnd,
                                 &DirectX9Interface::Margin);
  SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE,
                WS_EX_LAYERED | WS_EX_TRANSPARENT);
  ShowWindow(OverlayWindow::Hwnd, SW_SHOW);
  UpdateWindow(OverlayWindow::Hwnd);
}

void LogInfo() noexcept {
#ifdef NDEBUG
  system("cls");
#endif
  printf(
      XorStr("  ####    ##   #    # # #####    ##   #####  ###### \n"
             " #       #  #  ##  ## # #    #  #  #  #    # #      \n"
             "  ####  #    # # ## # # #    # #    # #    # #####  \n"
             "      # ###### #    # # #    # ###### #####  #      \n"
             " #    # #    # #    # # #    # #    # #   #  #      \n"
             "  ####  #    # #    # # #####  #    # #    # ###### \n"
             "\"\u4e94\u6708\u96e8\" for Counter-Strike 2\n"));
  printf(XorStr("Build - %s - %s\n"), __DATE__, __TIME__);
  printf(XorStr(
      "\u305d\u308c\u3067\u3082\u8ab0\u304b\u306b\u898b\u3064\u3051\u3066\u6b32"
      "\u3057\u304f\u3066"
      "\n\u591c\u7a7a\u898b\u4e0a\u3052\u3066\u53eb\u3093\u3067\u3044\u308b"
      "\n"));
  printf(XorStr("Menukey [DEL]/[INS]\n"));
  printf(XorStr("ProcessId: %d\nClientBase: %p\nEngineBase: %p\nTier0Base: %p\n"),
         global::processId,
         reinterpret_cast<void*>(gGame.GetClientDLLAddress()),
         reinterpret_cast<void*>(gGame.GetEngineDLLAddress()),
         reinterpret_cast<void*>(gGame.GetTier0DLLAddress()));
}

bool InitializeGameProcess() noexcept {
  Ukia::ProcessMgr.Attach(XorStr("cs2.exe"));
  const DWORD processId = Ukia::ProcessMgr.ProcessID;
  if (!processId) return false;
  if (!gGame.InitAddress()) {
    MessageBoxA(nullptr, XorStr("Failed to call InitAddress()."),
                XorStr("Samidare Error"), MB_ICONERROR);
    return false;
  }
  if (!Ukia::ProcessMgr.ReadMemory(gGame.GetBuildNumberAddress(),
                                   Vars::GameVersion)) {
    MessageBoxA(nullptr, XorStr("Failed to get game version."),
                XorStr("Samidare Error"), MB_ICONERROR);
    Ukia::UkiaExit();
  }
  global::processId = processId;
  global::hwnd_ = Ukia::ProcessMgr.GetWindowHandleFromProcessId(processId);

  LogInfo();
  return true;
}

class ScopedThreadManager {
 public:
  bool CreateThreads() noexcept {
    try {
      m_threads.emplace_back([&] { CheckAliveThread(); });
      m_threads.emplace_back([&] { DataUpdateThread(); });
      m_threads.emplace_back([&] { EntityUpdateThread(); });
      m_threads.emplace_back([&] { MapUpdateThread(); });
      m_threads.emplace_back([&] { AimProcessThread(); });
      m_threads.emplace_back([&] { ViewProcessThread(); });
      m_threads.emplace_back([&] { MemoryProcessThread(); });
      m_threads.emplace_back([&] { NonMemoryProcessThread(); });
      return true;
    } catch (const std::exception& e) {
      MessageBoxA(nullptr, e.what(), XorStr("Thread Creation Error"),
                  MB_ICONERROR);
      return false;
    }
  }

  ~ScopedThreadManager() noexcept {
    global::isRunning = false;
    for (auto& thread : m_threads) {
      if (thread.joinable()) thread.join();
    }
  }

 private:
  std::vector<std::thread> m_threads;

  void CheckAliveThread() noexcept {
    while (global::isRunning) {
      constexpr wchar_t EXPECTED_TITLE[] = L"Counter-Strike 2";
      wchar_t actualTitle[256] = {0};
      GetWindowTextW(global::hwnd_, actualTitle, _countof(actualTitle));
      constexpr wchar_t EXPECTED_TITLE_CN[] = L"\u53cd\u6050\u7cbe\u82f1\uff1a\u5168\u7403\u653b\u52bf";
      wchar_t actualTitleCN[256] = {0};
      GetWindowTextW(global::hwnd_, actualTitleCN, _countof(actualTitle));
      global::isRunning = ((wcscmp(actualTitle, EXPECTED_TITLE) ==0)||
          (wcscmp(actualTitleCN, EXPECTED_TITLE_CN) == 0));
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  void DataUpdateThread() noexcept {
    while (global::isRunning) {
      Vars::UpdateDataSlow();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(Vars::GlobalVarsInterval));
    }
  }
  void EntityUpdateThread() noexcept {
    while (global::isRunning) {
      Vars::UpdateData();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(Vars::EntityInterval));
    }
  }
  void MapUpdateThread() noexcept {
    while (global::isRunning) {
      Vars::ParserRun();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(Vars::ParserInterval));
    }
  }
  void AimProcessThread() noexcept {
    while (global::isRunning) {
      AimFunctions(Vars::ValidEntity);
      std::this_thread::sleep_for(std::chrono::milliseconds(Vars::AimInterval));
    }
  }
  void ViewProcessThread() noexcept {
    while (global::isRunning) {
      ViewFunctions(Vars::ValidEntity);
      std::this_thread::sleep_for(
          std::chrono::milliseconds(Vars::ViewInterval));
    }
  }
  void MemoryProcessThread() noexcept {
    while (global::isRunning) {
      MemoryFunctions(Vars::ValidEntity);
      std::this_thread::sleep_for(
          std::chrono::milliseconds(Vars::MemoryInterval));
    }
  }
  void NonMemoryProcessThread() noexcept {
    while (global::isRunning) {
      NonMemoryFunctions();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(Vars::NonMemoryInterval));
    }
  }
};

bool WaitForGameFocus() {
  constexpr auto focusCheckInterval = std::chrono::milliseconds(15);
  auto startTime = std::chrono::steady_clock::now();

  while (global::isRunning) {
    DWORD foregroundPID = 0;
    GetWindowThreadProcessId(GetForegroundWindow(), &foregroundPID);

    if (foregroundPID == global::processId) {
      JustGetWindowRect();
      return true;
    }

    if (std::chrono::steady_clock::now() - startTime >
        std::chrono::seconds(30)) {
      MessageBoxA(nullptr, XorStr("Focus wait timeout"),
                  XorStr("Samidare Error"), MB_ICONERROR);
      return false;
    }
    std::this_thread::sleep_for(focusCheckInterval);
  }
  return false;
}

bool InitializeRendering() noexcept {
  randomWindowName = Ukia::getRandomPoem().c_str();
  randomWindowNameW = Ukia::getRandomPoemW();
  OverlayWindow::Name = randomWindowName;
  SetupWindow();

  if (!DirectXInit()) {
    MessageBoxA(nullptr, XorStr("DirectX initialization failed"),
                XorStr("Samidare Error"), MB_ICONERROR);
    return false;
  }

  return true;
}

void RunMainLoop() noexcept {
  try {
    while (global::isRunning) {
      MainLoop();
    }
  } catch (const std::exception& e) {
    MessageBoxA(nullptr, e.what(), XorStr("Rendering Error"), MB_ICONERROR);
  }
}

void CleanupResources() noexcept {
  if (OverlayWindow::Hwnd) {
    DestroyWindow(OverlayWindow::Hwnd);
    UnregisterClassA(OverlayWindow::WindowClass.lpszClassName,
                     OverlayWindow::WindowClass.hInstance);
    Ukia::UkiaExit();
  }
}

int Mian() noexcept {
  global::uiAccessStatus = PrepareForUIAccess();

  char documentsPath[MAX_PATH];
  if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath) != S_OK) {
    MessageBoxA(nullptr, XorStr("Failed to get the Documents folder path."),
                XorStr("Samidare Error"), MB_ICONERROR);
    Ukia::UkiaExit();
  }

  config::path = documentsPath;
  config::path += XorStr("\\Samidare");
  LangSettings::path = config::path + XorStr("\\Langs");
  if (std::filesystem::exists(config::path)) {
    printf(XorStr("Config folder connected: %s\n"), config::path.c_str());
  } else {
    if (std::filesystem::create_directories(config::path)) {
      printf(XorStr("Config folder created: %s\n"), config::path.c_str());
    } else {
      MessageBoxA(nullptr, XorStr("Failed to create the config directory."),
                  XorStr("Samidare Error"), MB_ICONERROR);
      Ukia::UkiaExit();
    }
  }
  if (!Offset::UpdateOffsets()) {
    MessageBoxA(
        nullptr,
        XorStr("Failed to update offsets.\nPlease visit "
               "https://github.com/a2x/cs2-dumper/tree/main/output \nto get "
               "latest offsets.json & buttons.json & client_dll.json & "
               "interfaces.json\n"),
        XorStr("Samidare Error"), MB_ICONERROR);
    Ukia::UkiaExit();
  }
  if (std::filesystem::exists(LangSettings::path)) {
    printf(XorStr("Languages file folder connected: %s\n"),
           LangSettings::path.c_str());
  } else {
    if (std::filesystem::create_directories(LangSettings::path)) {
      printf(XorStr("Languages file folder created: %s\n"),
             LangSettings::path.c_str());
    } else {
      MessageBoxA(nullptr,
                  XorStr("Failed to create the languages file directory."),
                  XorStr("Samidare Error"), MB_ICONERROR);
      Ukia::UkiaExit();
    }
  }
  if (std::filesystem::exists(config::path + XorStr("\\Maps"))) {
    printf(XorStr("Maps folder connected: %s\n"),
           (config::path + XorStr("\\Maps\\")).c_str());
  } else {
    if (std::filesystem::create_directories(config::path + XorStr("\\Maps"))) {
      printf(XorStr("Maps folder created: %s\n"),
             (config::path + XorStr("\\Maps")).c_str());
    } else {
      MessageBoxA(nullptr, XorStr("Failed to create the Maps directory."),
                  XorStr("Samidare Error"), MB_ICONERROR);
      Ukia::UkiaExit();
    }
  }
  global::userName = std::getenv(XorStr("USERNAME"));

  if (!InitializeGameProcess()) {
    MessageBoxA(nullptr, XorStr("Failed to initialize game process"),
                XorStr("Samidare Error"), MB_ICONERROR);
    return -1;
  }

  global::isRunning = true;

  if (!WaitForGameFocus()) {
    MessageBoxA(nullptr, XorStr("Wait game window focus time out"),
                XorStr("Samidare Error"), MB_ICONERROR);
    return -1;
  }

  ScopedThreadManager threadManager;
  if (!threadManager.CreateThreads()) {
    MessageBoxA(nullptr, XorStr("Failed to create worker threads"),
                XorStr("Samidare Error"), MB_ICONERROR);
    return -1;
  }

  if (!InitializeRendering()) {
    MessageBoxA(nullptr, XorStr("Failed to initialize rendering system"),
                XorStr("Samidare Error"), MB_ICONERROR);
    return -1;
  }

  RunMainLoop();

  CleanupResources();
  return 0;
}

int main(int argc, char* argv[]) noexcept {
  Ukia::UkiaInit(argc, argv);
  return Mian();
}