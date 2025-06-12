#include "UkiaStuff.h"

#include <TlHelp32.h>
#include <malloc.h>

#include <atlconv.h>
#include <iphlpapi.h>
#include <psapi.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <vector>
#ifndef USERMODE
#ifndef FIFO_MODE
#include "driver.h"
#else
#include "FIFO.h"
#endif
#endif  // USERMODE
#include "XorStr.h"


#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "iphlpapi.lib")

constexpr uint32_t CompileTimeSeed() noexcept {
  const char* time_str = __TIME__ __DATE__;
  uint32_t hash = 0;
  for (int i = 0; time_str[i] != '\0'; ++i) {
    hash = hash * 65599 + time_str[i];
  }
  return hash;
}

namespace Ukia {
void HideConsole() noexcept {
  HWND hwndConsole = GetConsoleWindow();
  if (hwndConsole) {
    ShowWindow(hwndConsole, SW_SHOWMINNOACTIVE);  // 隐藏控制台
  }
}

void ShowConsole() noexcept {
  HWND hwndConsole = GetConsoleWindow();
  if (hwndConsole) {
    ShowWindow(hwndConsole, SW_SHOWNOACTIVATE);  // 显示控制台
  }
}
void AntiDebugger(std::string log) noexcept {
  if (IsDebuggerPresent()) {
    if (log != "")
      printf((log + "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
                    "\n\n\n\n\n\n\n\n\n\n\n\n")
                 .c_str());
    ShowWindow(GetConsoleWindow(), false);
    _exit(0);
  }
}

std::wstring utf8ToUtf16(const std::string& utf8Str) noexcept {
  int size_needed =
      MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
  if (size_needed == 0) {
    return std::wstring();
  }
  std::wstring wstr(size_needed, 0);
  MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], size_needed);
  return wstr;
}

std::string getRandomPoem() noexcept {
  static const std::vector<std::string> words = {
      "\xE9\xA3\x8E",  // 风
      "\xE8\x8A\xB1",  // 花
      "\xE9\x9B\xAA",  // 雪
      "\xE6\x9C\x88",  // 月
      "\xE7\x83\x9F",  // 烟
      "\xE6\x97\xB6",  // 时
      "\xE9\x9B\xA8",  // 雨
      "\xE6\xB8\x9F",  // 渟
      "\xE4\xBA\x91",  // 云
      "\xE6\xB1\x9F",  // 江
      "\xE6\xB9\x96",  // 湖
      "\xE6\x98\x9F",  // 星
      "\xE6\x9C\x88",  // 月
      "\xE6\x99\xAF",  // 景
      "\xE7\x94\x9F",  // 生
      "\xE6\xA2\x85",  // 梅
      "\xE6\x9F\xB3",  // 柳
      "\xE8\x93\x89",  // 莲
      "\xE5\xAE\x87",  // 宇
      "\xE5\xAE\x99",  // 宙
      "\xE6\x98\xA5",  // 春
      "\xE5\xA4\x8F",  // 夏
      "\xE7\xA7\x8B",  // 秋
      "\xE5\x86\xAC",  // 冬
      "\xE5\xB1\xB1",  // 山
      "\xE6\xB0\xB4"   // 水
  };

  static std::mt19937 rng(static_cast<unsigned>(
      std::chrono::system_clock::now().time_since_epoch().count()));

  std::vector<std::string> shuffled = words;
  std::shuffle(shuffled.begin(), shuffled.end(), rng);

  std::string poem;
  for (int i = 0; i < 10; ++i) {
    poem += shuffled[i];
    if ((i + 1) % 5 == 0 && i != 10 - 1) {
      poem += "\xEF\xBC\x8C";
    }
  }
  poem += "\xE3\x80\x82";
  return poem;
}

LPCWSTR getRandomPoemW() noexcept {
  std::string utf8Poem = getRandomPoem();
  std::wstring wstr = utf8ToUtf16(utf8Poem);
  static std::wstring staticWstr;
  staticWstr = wstr;
  return staticWstr.c_str();
}

std::string GetSelfPath() noexcept {
  char pathBuffer[MAX_PATH] = {0};

  if (GetModuleFileNameA(NULL, pathBuffer, sizeof(pathBuffer)) == 0) {
    perror("GetModuleFileNameA failed");
    return "";
  }
  return std::string(pathBuffer);
}

bool HasHashReadyParameter(int argc, char* argv[]) noexcept {
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--hash-ready") == 0) return true;
  }
  return false;
}

void PreUpdateHash(const std::string& exePath, char* argv[]) noexcept {
  size_t pos = exePath.find_last_of("\\/");
  std::string folder =
      (pos != std::string::npos) ? exePath.substr(0, pos + 1) : "";
  std::string exeName =
      (pos != std::string::npos) ? exePath.substr(pos + 1) : exePath;
  srand(static_cast<unsigned int>(time(nullptr)));
  long long randomNum = (static_cast<long long>(rand() * 2654435761u));
  std::ostringstream oss;
  oss << folder << "ukiaUpd_" << std::hex << randomNum << ".bat";
  std::string batPath = oss.str();

  std::ofstream batFile(batPath);
  if (!batFile) {
    fprintf(stderr, "failed create shell: %s\n", batPath.c_str());
    return;
  }

  batFile << "@echo off\n";
  batFile
      << "powershell -NoProfile -Command \""
      << "$exePath = '%~dp0" << exeName << "'; "
      << "$fs = [System.IO.File]::Open($exePath, [System.IO.FileMode]::Open, "
         "[System.IO.FileAccess]::ReadWrite); "
      << "try { "
      << "  $fs.Seek(0, [System.IO.SeekOrigin]::End) | Out-Null; "
      << "  $length = $fs.Length; "
      << "  $bufferSize = [Math]::Min(1024, $length); "
      << "  $fs.Seek(-$bufferSize, [System.IO.SeekOrigin]::End) | Out-Null; "
      << "  $buffer = New-Object Byte[] $bufferSize; "
      << "  $fs.Read($buffer, 0, $bufferSize) | Out-Null; "
      << "  $text = [System.Text.Encoding]::ASCII.GetString($buffer); "
      << "  if($text -match '##TE_QUIERO_MUCHO##(.*?)##UKIA_LOVES_YOU##') { "
      << "      $newHash = " << randomNum << " ; "
      << "      $newBlock = '##TE_QUIERO_MUCHO##' + $newHash + "
         "'##UKIA_LOVES_YOU##'; "
      << "      $match = [System.Text.RegularExpressions.Regex]::Match($text, "
         "'##TE_QUIERO_MUCHO##.*?##UKIA_LOVES_YOU##'); "
      << "      if($match.Success) { "
      << "          $matchIndex = $match.Index; "
      << "          $absoluteIndex = $length - $bufferSize + $matchIndex; "
      << "          $fs.SetLength($absoluteIndex); "
      << "          $writer = New-Object System.IO.BinaryWriter($fs); "
      << "          "
         "$writer.Write([System.Text.Encoding]::ASCII.GetBytes($newBlock)); "
      << "          $writer.Flush(); "
      << "          $writer.Close(); "
      << "      } "
      << "  } else { "
      << "      $newHash = " << randomNum << " ; "
      << "      $newBlock = '##TE_QUIERO_MUCHO##' + $newHash + "
         "'##UKIA_LOVES_YOU##'; "
      << "      $writer = New-Object System.IO.BinaryWriter($fs); "
      << "      "
         "$writer.Write([System.Text.Encoding]::ASCII.GetBytes($newBlock)); "
      << "      $writer.Flush(); "
      << "      $writer.Close(); "
      << "  } "
      << "} finally { $fs.Close(); }\" \n";
  batFile << "start \"\" \"%~dp0" << exeName << "\" --hash-ready " << argv
          << "\n ";
  batFile << "del \"%~f0\"\n";
  batFile.close();

  ShellExecuteA(NULL, "open", batPath.c_str(), NULL,
                folder.empty() ? NULL : folder.c_str(), SW_HIDE);
}
void PostUpdateHash(const std::string& exePath) noexcept {
  size_t pos = exePath.find_last_of("\\/");
  std::string folder =
      (pos != std::string::npos) ? exePath.substr(0, pos + 1) : "";
  std::string exeName =
      (pos != std::string::npos) ? exePath.substr(pos + 1) : exePath;
  srand(static_cast<unsigned int>(time(nullptr)));
  long long randomNum = (static_cast<long long>(rand() * 2654435761u));
  std::ostringstream oss;
  oss << folder << "ukiaUpd_" << std::hex << randomNum << ".bat";
  std::string batPath = oss.str();

  std::ofstream batFile(batPath);
  if (!batFile) {
    fprintf(stderr, "failed create shell: %s\n", batPath.c_str());
    return;
  }

  batFile << "@echo off\n";
  batFile << "timeout /t 0.5 /nobreak >nul\n";
  batFile
      << "powershell -NoProfile -Command \""
      << "$exePath = '%~dp0" << exeName << "'; "
      << "$fs = [System.IO.File]::Open($exePath, [System.IO.FileMode]::Open, "
         "[System.IO.FileAccess]::ReadWrite); "
      << "try { "
      << "  $fs.Seek(0, [System.IO.SeekOrigin]::End) | Out-Null; "
      << "  $length = $fs.Length; "
      << "  $bufferSize = [Math]::Min(1024, $length); "
      << "  $fs.Seek(-$bufferSize, [System.IO.SeekOrigin]::End) | Out-Null; "
      << "  $buffer = New-Object Byte[] $bufferSize; "
      << "  $fs.Read($buffer, 0, $bufferSize) | Out-Null; "
      << "  $text = [System.Text.Encoding]::ASCII.GetString($buffer); "
      << "  if($text -match '##TE_QUIERO_MUCHO##(.*?)##UKIA_LOVES_YOU##') { "
      << "      $newHash = " << randomNum << " ; "
      << "      $newBlock = '##TE_QUIERO_MUCHO##' + $newHash + "
         "'##UKIA_LOVES_YOU##'; "
      << "      $match = [System.Text.RegularExpressions.Regex]::Match($text, "
         "'##TE_QUIERO_MUCHO##.*?##UKIA_LOVES_YOU##'); "
      << "      if($match.Success) { "
      << "          $matchIndex = $match.Index; "
      << "          $absoluteIndex = $length - $bufferSize + $matchIndex; "
      << "          $fs.SetLength($absoluteIndex); "
      << "          $writer = New-Object System.IO.BinaryWriter($fs); "
      << "          "
         "$writer.Write([System.Text.Encoding]::ASCII.GetBytes($newBlock)); "
      << "          $writer.Flush(); "
      << "          $writer.Close(); "
      << "      } "
      << "  } else { "  // idk, but if he would deleted it manually? haha
      << "      $newHash = " << randomNum << " ; "
      << "      $newBlock = '##TE_QUIERO_MUCHO##' + $newHash + "
         "'##UKIA_LOVES_YOU##'; "
      << "      $writer = New-Object System.IO.BinaryWriter($fs); "
      << "      "
         "$writer.Write([System.Text.Encoding]::ASCII.GetBytes($newBlock)); "
      << "      $writer.Flush(); "
      << "      $writer.Close(); "
      << "  } "
      << "} finally { $fs.Close(); }\" \n";
  batFile << "del \"%~f0\"\n";
  batFile.close();

  ShellExecuteA(NULL, "open", batPath.c_str(), NULL,
                folder.empty() ? NULL : folder.c_str(), SW_HIDE);
}
void RandomTitle() noexcept {
  LPCWSTR title = getRandomPoemW();
  SetConsoleTitle(title);
}

bool IsFullscreen(HWND hwnd) noexcept {
  RECT windowRect, screenRect;
  GetWindowRect(hwnd, &windowRect);
  GetWindowRect(GetDesktopWindow(), &screenRect);
  return (windowRect.left <= screenRect.left &&
          windowRect.top <= screenRect.top &&
          windowRect.right >= screenRect.right &&
          windowRect.bottom >= screenRect.bottom);
}

std::string GenerateMacAddress() noexcept {
  IP_ADAPTER_INFO AdapterInfo[16];
  DWORD dwBufLen = sizeof(AdapterInfo);
  DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);

  if (dwStatus != ERROR_SUCCESS) {
    return "Error";
  }

  PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
  std::stringstream macAddress;

  do {
    macAddress << std::hex << std::uppercase << std::setw(2)
               << std::setfill('0')
               << static_cast<int>(pAdapterInfo->Address[0]) << std::setw(2)
               << std::setfill('0')
               << static_cast<int>(pAdapterInfo->Address[1]) << std::setw(2)
               << std::setfill('0')
               << static_cast<int>(pAdapterInfo->Address[2]) << std::setw(2)
               << std::setfill('0')
               << static_cast<int>(pAdapterInfo->Address[3]) << std::setw(2)
               << std::setfill('0')
               << static_cast<int>(pAdapterInfo->Address[4]) << std::setw(2)
               << std::setfill('0')
               << static_cast<int>(pAdapterInfo->Address[5]);

    pAdapterInfo = pAdapterInfo->Next;
  } while (pAdapterInfo);

  return macAddress.str();
}

std::string GenerateDiskSerial() noexcept {
  DWORD serialNum;
  GetVolumeInformationA("C:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);

  std::stringstream ss;
  ss << serialNum;
  return ss.str();
}

static std::string GenerateHwId() noexcept {
  std::string strMac = GenerateMacAddress();
  std::string strDiskSerial = GenerateDiskSerial();
  return strMac + strDiskSerial;
}

// simple function i made that will just initialize our Object_Attributes
// structure as NtOpenProcess will fail otherwise
#ifdef USERMODE
OBJECT_ATTRIBUTES InitObjectAttributes(PUNICODE_STRING name, ULONG attributes,
                                       HANDLE hRoot,
                                       PSECURITY_DESCRIPTOR security) noexcept {
  OBJECT_ATTRIBUTES object;

  object.Length = sizeof(OBJECT_ATTRIBUTES);
  object.ObjectName = name;
  object.Attributes = attributes;
  object.RootDirectory = hRoot;
  object.SecurityDescriptor = security;

  return object;
}


HANDLE UkiaOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle,
                       DWORD dwProcessId) noexcept {
  // At last is same with "OpenProcess", but we call "NtOpenProcess" (Native
  // API) directly.
  HANDLE hProcess = 0;
  _NtOpenProcess NtOpenProcess = (_NtOpenProcess)GetProcAddress(
      GetModuleHandleA(XorStr("ntdll.dll")), XorStr("NtOpenProcess"));
  CLIENT_ID clientId = {(HANDLE)dwProcessId, NULL};
  OBJECT_ATTRIBUTES objAttr = InitObjectAttributes(NULL, 0, NULL, NULL);
  SPOOF_FUNC;
  SPOOF_CALL(NtOpenProcess)(&hProcess, dwDesiredAccess, &objAttr, &clientId);
  return hProcess;
}

inline static pNtReadVirtualMemory NtReadVirtualMemory = []() {
  return reinterpret_cast<pNtReadVirtualMemory>(GetProcAddress(
      GetModuleHandleA(XorStr("ntdll.dll")), XorStr("NtReadVirtualMemory")));
}();
inline static pNtWriteVirtualMemory NtWriteVirtualMemory = []() {
  return reinterpret_cast<pNtWriteVirtualMemory>(GetProcAddress(
      GetModuleHandleA(XorStr("ntdll.dll")), XorStr("NtWriteVirtualMemory")));
}();
using RtlNtStatusToDosErrorFn = ULONG(WINAPI*)(NTSTATUS);
inline static auto RtlNtStatusToDosError = reinterpret_cast<RtlNtStatusToDosErrorFn>(
    GetProcAddress(GetModuleHandleA(XorStr("ntdll.dll")),
                   XorStr("RtlNtStatusToDosError")));
#endif
BOOL UkiaReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress,
                           LPVOID lpBuffer, SIZE_T nSize,
                           SIZE_T* lpNumberOfBytesRead) noexcept {
#ifndef USERMODE
#ifndef FIFO_MODE
  driver.readsize((uintptr_t)lpBaseAddress, lpBuffer, nSize);
  return TRUE;
#else
  fifo.readSize((uintptr_t)lpBaseAddress, lpBuffer, nSize);
  return TRUE;
#endif
#else
  if (!NtReadVirtualMemory) {
    SetLastError(ERROR_PROC_NOT_FOUND);
    return FALSE;
  }

  ULONG ulSize = static_cast<ULONG>(nSize);
  ULONG bytesRead = 0;
  SPOOF_FUNC;
  NTSTATUS status = SPOOF_CALL(NtReadVirtualMemory)(
      hProcess, const_cast<PVOID>(lpBaseAddress), lpBuffer,
                          ulSize, lpNumberOfBytesRead ? &bytesRead : nullptr);

  if (lpNumberOfBytesRead) {
    *lpNumberOfBytesRead = static_cast<SIZE_T>(bytesRead);
  }

  if (status == SUCCEED) {
    return TRUE;
  }

  if (status == 0x8000000D) {
    SetLastError(ERROR_PARTIAL_COPY);
    return FALSE;
  }

  if (RtlNtStatusToDosError) {
    SetLastError(RtlNtStatusToDosError(status));
  } else {
    SetLastError(ERROR_UNIDENTIFIED_ERROR);
  }

  return FALSE;
  #endif
}

BOOL UkiaWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress,
                            LPCVOID lpBuffer, SIZE_T nSize,
                            SIZE_T* lpNumberOfBytesWritten) noexcept {
#ifndef USERMODE
#ifndef FIFO_MODE
  driver.write((uintptr_t)lpBaseAddress, lpBuffer, nSize);
  return TRUE;
  #else
   fifo.write((uintptr_t)lpBaseAddress, lpBuffer, nSize);
  return TRUE;
#endif
    #else
  if (!NtWriteVirtualMemory) {
    SetLastError(ERROR_PROC_NOT_FOUND);
    return FALSE;
  }

  ULONG ulSize = static_cast<ULONG>(nSize);
  ULONG bytesWritten = 0;
  SPOOF_FUNC;
  NTSTATUS status = SPOOF_CALL(NtWriteVirtualMemory)(
      hProcess, lpBaseAddress, const_cast<PVOID>(lpBuffer), ulSize,
      lpNumberOfBytesWritten ? &bytesWritten : nullptr);

  if (lpNumberOfBytesWritten) {
    *lpNumberOfBytesWritten = static_cast<SIZE_T>(bytesWritten);
  }

  if (status == SUCCEED) {
    return TRUE;
  }

  if (status == 0xC0000005)  // STATUS_ACCESS_VIOLATION
    SetLastError(ERROR_NOACCESS);
  if (status == 0xC0000022)  // STATUS_ACCESS_DENIED
    SetLastError(ERROR_ACCESS_DENIED);
  if (status == 0x8000000D)
    SetLastError(ERROR_WRITE_FAULT);
  else {
    if (RtlNtStatusToDosError) {
      SetLastError(RtlNtStatusToDosError(status));
    } else {
      SetLastError(ERROR_UNIDENTIFIED_ERROR);
    }
  }

  return FALSE;
  #endif
}

BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) noexcept {
  switch (dwCtrlType) {
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
#ifdef NDEBUG
      Ukia::AntiDebugger(XorStr("Initialize fail"));
      if (true) {
        ProcessMgr.Detach();
        std::string selfPath = Ukia::GetSelfPath();
        if (selfPath.empty()) {
          return EXIT_FAILURE;
        }
        Ukia::PostUpdateHash(selfPath);
        _exit(0);
        return 0;
      }
#endif
      Sleep(2000);
      return FALSE;
    default:
      break;
  }
  return FALSE;
}
StatusCode ProcessManager::Attach(std::string ProcessName) noexcept {

  ProcessID = this->GetProcessID(ProcessName);
  IS_INVALID_R(ProcessID, FAILE_PROCESSID);
#ifndef USERMODE
#ifndef FIFO_MODE
  driver.initdriver(ProcessID);
  ModuleAddress = (uintptr_t) driver.dll_address("client.dll");
  IS_INVALID_R(ModuleAddress, FAILE_MODULE);
  attached_ = true;

  return SUCCEED;
  #else
  fifo.initPipe(ProcessID);
  ModuleAddress = (uintptr_t)fifo.dllAddress("client.dll");
  IS_INVALID_R(ModuleAddress, FAILE_MODULE);
  attached_ = true;

  return SUCCEED;
  #endif
  #else
  ModuleAddress =
      reinterpret_cast<DWORD64>(this->GetProcessModuleHandle(ProcessName));
  IS_INVALID_R(ModuleAddress, FAILE_MODULE);

  // hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ |
  // PROCESS_VM_WRITE, FALSE, ProcessID);

  hProcess = UkiaOpenProcess(
      PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ
      // | PROCESS_VM_WRITE
      ,
      FALSE, ProcessID);

  if (GetProcessId(hProcess) == ProcessID) {
    attached_ = true;
    delete[] t_SYSTEM_HANDLE_INFORMATION;
  } else {
    CloseHandle(hProcess);
    return FAILE_HPROCESS;
  }
  return SUCCEED;
  #endif
}

void ProcessManager::Detach() noexcept {
#ifdef USERMODE
  if (hProcess) CloseHandle(hProcess);
#endif

  hProcess = 0;
  ProcessID = 0;
  ModuleAddress = 0;
  attached_ = false;
}
HWND ProcessManager::GetWindowHandleFromProcessId(DWORD ProcessId) noexcept {
  HWND hwnd = NULL;
  do {
    hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == ProcessId) {
      TCHAR windowTitle[MAX_PATH];
      GetWindowText(hwnd, windowTitle, MAX_PATH);
      if (IsWindowVisible(hwnd) && windowTitle[0] != '\0') {
        return hwnd;
      }
    }
  } while (hwnd != NULL);
  return NULL;  // No main window found for the given process ID
}
/*
bool ProcessManager::IsActive() noexcept {
  if (!attached_) return false;
  DWORD ExitCode{};
  GetExitCodeProcess(hProcess, &ExitCode);
  return ExitCode == STILL_ACTIVE;
}
*/
// tewshi0 idea
std::string ProcessManager::ReadString(DWORD64 address,
                                       size_t maxLength) noexcept {
  std::vector<char> buffer(maxLength, 0);

  if (!ReadMemory<char>(address, buffer[0], maxLength)) {
    return "";
  }

  buffer[maxLength - 1] = '\0';

  size_t actualLength = 0;
  while (actualLength < maxLength && buffer[actualLength] != '\0') {
    ++actualLength;
  }

  return std::string(buffer.data(), actualLength);
}

DWORD ProcessManager::GetProcessID(std::string ProcessName) noexcept {
  PROCESSENTRY32 ProcessInfoPE;
  ProcessInfoPE.dwSize = sizeof(PROCESSENTRY32);
  SPOOF_FUNC;
  HANDLE hSnapshot = SPOOF_CALL(CreateToolhelp32Snapshot)(15, 0);
  Process32First(hSnapshot, &ProcessInfoPE);
  USES_CONVERSION;
  do {
    if (strcmp(W2A(ProcessInfoPE.szExeFile), ProcessName.c_str()) == 0) {
      CloseHandle(hSnapshot);
      return ProcessInfoPE.th32ProcessID;
    }
  } while (Process32Next(hSnapshot, &ProcessInfoPE));
  SPOOF_CALL(CloseHandle)(hSnapshot);
  return 0;
}
DWORD64 ProcessManager::TraceAddress(DWORD64 BaseAddress,
                                     std::vector<DWORD> Offsets) noexcept {
#ifdef USERMODE
  IS_INVALID_R(hProcess, 0);
#endif
  IS_INVALID_R(ProcessID, 0);
  DWORD64 Address = 0;

  if (Offsets.size() == 0) return BaseAddress;

  if (!ReadMemory<DWORD64>(BaseAddress, Address)) return 0;

  for (int i = 0; i < Offsets.size() - 1; i++) {
    if (!ReadMemory<DWORD64>(Address + Offsets[i], Address)) return 0;
  }
  return Address == 0 ? 0 : Address + Offsets[Offsets.size() - 1];
}

HMODULE ProcessManager::GetProcessModuleHandle(
    std::string ModuleName) noexcept {
#ifndef USERMODE
#ifndef FIFO_MODE
    return (HMODULE)driver.dll_address(ModuleName.c_str());
#else
  return (HMODULE)fifo.dllAddress(ModuleName.c_str());
  #endif
    #else
  MODULEENTRY32 ModuleInfoPE;
  ModuleInfoPE.dwSize = sizeof(MODULEENTRY32);
  SPOOF_FUNC;
  HANDLE hSnapshot =
      SPOOF_CALL(CreateToolhelp32Snapshot)(TH32CS_SNAPMODULE, this->ProcessID);
  Module32First(hSnapshot, &ModuleInfoPE);
  USES_CONVERSION;
  do {
    if (strcmp(W2A(ModuleInfoPE.szModule), ModuleName.c_str()) == 0) {
      SPOOF_CALL(CloseHandle)(hSnapshot);
      return ModuleInfoPE.hModule;
    }
  } while (Module32Next(hSnapshot, &ModuleInfoPE));
  SPOOF_CALL(CloseHandle)(hSnapshot);
  return 0;
  #endif
}

int UkiaInit(int argc, char* argv[]) noexcept {
  std::locale::global(std::locale("en_HK.UTF-8"));
#ifdef NDEBUG
  Ukia::HideConsole();
  Ukia::AntiDebugger(XorStr("Initialize fail"));
  if (!HasHashReadyParameter(argc, argv)) {
    std::string selfPath = GetSelfPath();
    if (selfPath.empty()) {
      return EXIT_FAILURE;
    }
    Ukia::PreUpdateHash(selfPath, argv);
    _exit(0);
  }
  SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
#endif
  Ukia::ShowConsole();
  HANDLE hConsole =
      GetStdHandle(STD_OUTPUT_HANDLE);  // Gets a standard output device handle

  srand(static_cast<unsigned int>(time(nullptr)));
  RandomTitle();
  int iPadding = RANDOM_PADDING + (int)__FILE__ + __LINE__;
  // So that we can get randon .exe file Hash even codes are 100% same.
  printf(XorStr("%d\n"), iPadding);
  system("cls");
  printf(XorStr("Build - %s - %s\n"), __DATE__, __TIME__);
  UkiaData::strHWID = Ukia::GenerateHwId();
  printf(XorStr("%s\n"),
         UkiaData::strHWID.substr(UkiaData::strHWID.length() - 16).c_str());

  return 0;
}

int UkiaExit(DWORD code) noexcept {
  ProcessMgr.Detach();
  std::string selfPath = Ukia::GetSelfPath();
  if (selfPath.empty()) {
    return EXIT_FAILURE;
  }
  Ukia::PostUpdateHash(selfPath);
  _exit(code);
}
}  // namespace Ukia