#pragma once
#include "CallStack-Spoofer.h"
#include <Windows.h>

#include <string>
#include <vector>
#ifndef M_PI
#define M_PI 3.14159265358979323846  // matches value in gcc v2 math.h
#endif
#ifndef M_PI_F
#define M_PI_F ((float)(M_PI))  // Shouldn't collide with anything.
#endif
#ifndef M_PHI
#define M_PHI 1.61803398874989484820  // golden ratio
#endif
#ifndef RANDOM_PADDING
#define RANDOM_PADDING rand()
#endif
#ifndef IS_INVALID
#define IS_INVALID(v) \
  if (v == NULL) return false
#define IS_INVALID_R(v, n) \
  if (v == NULL) return n
#endif
enum StatusCode {
  SUCCEED,
  FAILE_PROCESSID,
  FAILE_HPROCESS,
  FAILE_MODULE,
};
typedef struct _UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWCH Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
  ULONG Length;
  HANDLE RootDirectory;
  PUNICODE_STRING ObjectName;
  ULONG Attributes;
  PVOID SecurityDescriptor;
  PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID {
  PVOID UniqueProcess;
  PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
  ULONG ProcessId;
  BYTE ObjectTypeNumber;
  BYTE Flags;
  USHORT Handle;
  PVOID Object;
  ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
  ULONG HandleCount;
  SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef NTSTATUS(NTAPI* _NtDuplicateObject)(HANDLE SourceProcessHandle,
                                            HANDLE SourceHandle,
                                            HANDLE TargetProcessHandle,
                                            PHANDLE TargetHandle,
                                            ACCESS_MASK DesiredAccess,
                                            ULONG Attributes, ULONG Options);

typedef NTSTATUS(NTAPI* _RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable,
                                             BOOLEAN CurrentThread,
                                             PBOOLEAN Enabled);

typedef NTSYSAPI NTSTATUS(NTAPI* _NtOpenProcess)(
    PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId);

typedef NTSTATUS(NTAPI* _NtQuerySystemInformation)(
    ULONG
        SystemInformationClass,  // your supposed to supply the whole class but
                                 // microsoft kept the enum mostly empty so I
                                 // just passed 16 instead for handle info.
                                 // Thats why you get a warning in your code btw
    PVOID SystemInformation, ULONG SystemInformationLength,
    PULONG ReturnLength);

typedef NTSTATUS(WINAPI* pNtReadVirtualMemory)(HANDLE ProcessHandle,
                                               PVOID BaseAddress, PVOID Buffer,
                                               ULONG NumberOfBytesToRead,
                                               PULONG NumberOfBytesRead);

typedef NTSTATUS(WINAPI* pNtWriteVirtualMemory)(HANDLE ProcessHandle,
                                                PVOID BaseAddress, PVOID Buffer,
                                                ULONG NumberOfBytesToWrite,
                                                PULONG NumberOfBytesWritten);

inline SYSTEM_HANDLE_INFORMATION* hInfo;  // holds the handle information
namespace UkiaData {
inline std::string strHWID = "";
}
constexpr uint32_t CompileTimeSeed() noexcept;
namespace Ukia {
void HideConsole() noexcept;
void ShowConsole() noexcept;
void AntiDebugger(std::string log = "") noexcept;
std::wstring utf8ToUtf16(const std::string& utf8Str) noexcept;
std::string getRandomPoem() noexcept;
LPCWSTR getRandomPoemW() noexcept;
std::string GetSelfPath() noexcept;
bool HasHashReadyParameter(int argc, char* argv[]) noexcept;
void PreUpdateHash(const std::string& exePath, char* argv[]) noexcept;
void PostUpdateHash(const std::string& exePath) noexcept;
void RandomTitle() noexcept;
bool IsFullscreen(HWND hwnd) noexcept;
std::string GenerateMacAddress() noexcept;
std::string GenerateDiskSerial() noexcept;
std::string GenerateHwId() noexcept;
HANDLE UkiaOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle,
                       DWORD dwProcessId) noexcept;
BOOL UkiaReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress,
                           LPVOID lpBuffer, SIZE_T nSize,
                           SIZE_T* lpNumberOfBytesRead) noexcept;
BOOL UkiaWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress,
                            LPCVOID lpBuffer, SIZE_T nSize,
                            SIZE_T* lpNumberOfBytesWritten) noexcept;

class ProcessManager {
 private:
  bool attached_ = false;

 public:
  HANDLE hProcess = 0;
  DWORD ProcessID = 0;
  DWORD64 ModuleAddress = 0;

 public:
  inline ~ProcessManager() noexcept {
    if (hProcess) {
      SPOOF_FUNC;
      SPOOF_CALL(CloseHandle)(hProcess);
    }
  }

  SYSTEM_HANDLE_INFORMATION* t_SYSTEM_HANDLE_INFORMATION;

  StatusCode Attach(std::string ProcessName) noexcept;
  void Detach() noexcept;
  HWND GetWindowHandleFromProcessId(DWORD ProcessId) noexcept;
  //bool IsActive() noexcept;
  template <typename ReadType>
  inline bool ReadMemory(DWORD64 Address, ReadType& Value, int Size) noexcept {
#ifdef USERMODE
    IS_INVALID(hProcess);
#endif
    IS_INVALID(ProcessID);

    if (UkiaReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address),
                              &Value, Size, 0))
      return true;
    return false;
  }

  template <typename ReadType>
  inline bool ReadMemory(DWORD64 Address, ReadType& Value) noexcept {
#ifdef USERMODE
    IS_INVALID(hProcess);
#endif
    IS_INVALID(ProcessID);

    if (UkiaReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address),
                              &Value, sizeof(ReadType), 0))
      return true;
    return false;
  }

  template <typename ReadType>
  inline bool WriteMemory(DWORD64 Address, ReadType& Value, int Size) noexcept {
#ifdef USERMODE
    IS_INVALID(hProcess);
#endif
    IS_INVALID(ProcessID);

    if (UkiaWriteProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address),
                               &Value, Size, 0))
      return true;
    return false;
  }

  template <typename ReadType>
  inline bool WriteMemory(DWORD64 Address, ReadType& Value) noexcept {
#ifdef USERMODE
    IS_INVALID(hProcess);
#endif
    IS_INVALID(ProcessID);

    if (UkiaWriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(Address),
                               &Value, sizeof(ReadType), 0))
      return true;
    return false;
  }
  std::string ReadString(DWORD64 address, size_t maxLength = 256) noexcept;
  DWORD GetProcessID(std::string ProcessName) noexcept;
  DWORD64 TraceAddress(DWORD64 BaseAddress,
                       std::vector<DWORD> Offsets) noexcept;
  template <typename T>
  inline bool GetDataAddressWithOffset(const DWORD64& Address,
                                                DWORD Offset,
                                                T& Data) noexcept {
    if (Address == 0) return false;

    if (!ReadMemory<T>(Address + Offset, Data)) return false;

    return true;
  }
  HMODULE GetProcessModuleHandle(std::string ModuleName) noexcept;
};
inline ProcessManager ProcessMgr;

BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) noexcept;
int UkiaInit(int argc, char* argv[]) noexcept;
int UkiaExit(DWORD code = 0) noexcept;
}  // namespace Ukia