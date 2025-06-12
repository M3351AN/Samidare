#pragma once
#include <fileapi.h>
#include <handleapi.h>
#include <ioapiset.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <vadefs.h>
#include <winioctl.h>

#include <cstdint>
#include <cstdio>
#include <string>

constexpr ULONG ioctl_call_driver =
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x775, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

class _driver {
 private:
  HANDLE _driver_handle;
  UINT64 _processid;
  UINT64 _cur_processid;
  ULONG64 _dlladdress;

// request codes
#define DRIVER_READVM 0xCAFE1
#define DRIVER_WRITEVM 0xCAFE2
#define HID 0xCAFE3
#define DLL_BASE 0xCAFE4

#pragma pack(push, 1)
  typedef struct _FixedStr64 {
    uint64_t blocks[4];
  } FixedStr64;
#pragma pack(pop)
  inline void encodeFixedStr64(const char* str, FixedStr64* fs) {
    size_t len = strlen(str);
    if (len > 32) {
      len = 32;
    }

    memset(fs->blocks, 0, sizeof(fs->blocks));

    for (size_t i = 0; i < len; i++) {
      size_t blockIndex = i / 8;
      size_t posInBlock = i % 8;
      int shift = 8 * (7 - posInBlock);
      fs->blocks[blockIndex] |= ((uint64_t)(unsigned char)str[i]) << shift;
    }
  }

  inline void decodeFixedStr64(const FixedStr64* fs, char* output,
                               size_t origLen) {
    size_t idx = 0;
    for (size_t block = 0; block < 4; block++) {
      for (int i = 0; i < 8; i++) {
        if (idx >= origLen) break;
        int shift = 8 * (7 - i);
        output[idx++] = (char)((fs->blocks[block] >> shift) & 0xFF);
      }
    }
    output[origLen] = '\0';
  }
#pragma pack(push, 1)
  typedef struct _Requests {
    // function requests
    int request_key;

    // memory read/write
    UINT64 src_pid;
    UINT64 src_addr;
    UINT64 dst_pid;
    UINT64 dst_addr;
    size_t size;

    // mouse_event
    DWORD dwFlags;
    DWORD dx;
    DWORD dy;
    DWORD dwData;
    ULONG_PTR dwExtraInfo;

    // return value
    UINT64 dll_base;

    // dllbase request
    FixedStr64 dll_name;
    SIZE_T dll_name_length;
  } Requests;
#pragma pack(pop)
  auto readvm(uint32_t src_pid, uint64_t src_addr, uint64_t dst_addr,
              size_t size) -> void {
    if (src_pid == 0 || src_addr == 0) return;

    Requests out = {DRIVER_READVM,  src_pid,  src_addr,
                    _cur_processid, dst_addr, size};

    DeviceIoControl(_driver_handle, ioctl_call_driver, &out, sizeof(out),
                    nullptr, 0, nullptr, nullptr);
  }
  auto writevm(uint32_t src_pid, uint64_t src_addr, uint64_t dst_addr,
               size_t size) -> void {
    if (src_pid == 0 || dst_addr == 0) return;

    Requests out = {DRIVER_WRITEVM, src_pid,  src_addr,
                    _cur_processid, dst_addr, size};

    DeviceIoControl(_driver_handle, ioctl_call_driver, &out, sizeof(out),
                    nullptr, 0, nullptr, nullptr);
  }

 public:
  auto initdriver(UINT64 processid) -> void {
    _driver_handle = CreateFileA("\\\\.\\Usugum0", GENERIC_READ, 0, nullptr,
                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                                 nullptr);  // get a handle to our driver
    if (_driver_handle != INVALID_HANDLE_VALUE) {
      _processid = processid;
      _cur_processid = GetCurrentProcessId();
    }
  }

  void readsize(const uintptr_t address, const void* buffer,
                const size_t size) {
    readvm(_processid, address, uintptr_t(buffer), size);
  }

  template <typename T>
  void write(const uintptr_t address, const T& buffer, int Size) {
    writevm(_processid, address, (uintptr_t)&buffer, Size);
  }
  auto dll_address(const char* dllname) -> ULONG64 {
    Requests out = {0};
    out.request_key = DLL_BASE;
    out.src_pid = _processid;

    size_t originalLen = strlen(dllname);
    if (originalLen > 32) originalLen = 32;
    out.dll_name_length = originalLen;
    FixedStr64 fs;
    encodeFixedStr64(dllname, &fs);
    out.dll_name = fs;

    DeviceIoControl(_driver_handle, ioctl_call_driver, &out, sizeof(out), &out,
                    sizeof(out), nullptr, nullptr);

    return out.dll_base;
  }

  void mouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData,
                   ULONG_PTR dwExtraInfo) {
    Requests request = {0};
    request.request_key = HID;
    request.dwFlags = dwFlags;
    request.dx = dx;
    request.dy = dy;
    request.dwData = dwData;
    request.dwExtraInfo = dwExtraInfo;

    DeviceIoControl(_driver_handle, ioctl_call_driver, &request,
                    sizeof(request), nullptr, 0, nullptr, nullptr);
  }
};

inline _driver driver;



