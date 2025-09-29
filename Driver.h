// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: Driver.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-30
//
// Description:
//   This file contains communication between Samidare and Usugumo.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef DRIVER_H_
#define DRIVER_H_
#include "pch.h"

#include <fileapi.h>
#include <handleapi.h>
#include <ioapiset.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <vadefs.h>
#include <winioctl.h>

constexpr ULONG ioctl_call_driver =
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x775, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

class _driver {
 private:
  HANDLE _driver_handle;
  UINT64 _processid;
  UINT64 _cur_processid;
  // ULONG64 _dlladdress;

// request codes
static const DWORD kDriverReadVM = 0xCAFE1;
static const DWORD kDriverWriteVM = 0xCAFE2;
static const DWORD kDriverEmulateHID = 0xCAFE3;
static const DWORD kDriverFetchModuleBase = 0xCAFE4;

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
      int shift = static_cast<int>(8 * (7 - posInBlock));
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
    UINT64 kDriverFetchModuleBase;

    // dllbase request
    FixedStr64 dll_name;
    SIZE_T dll_name_length;
  } Requests;
#pragma pack(pop)
  auto readvm(uint32_t src_pid, uint64_t src_addr, uint64_t dst_addr,
              size_t size) -> void {
    if (src_pid == 0 || src_addr == 0) return;

    Requests out = {kDriverReadVM,  src_pid,  src_addr,
                    _cur_processid, dst_addr, size};

    DeviceIoControl(_driver_handle, ioctl_call_driver, &out, sizeof(out),
                    nullptr, 0, nullptr, nullptr);
  }
  auto writevm(uint32_t src_pid, uint64_t src_addr, uint64_t dst_addr,
               size_t size) -> void {
    if (src_pid == 0 || dst_addr == 0) return;

    Requests out = {kDriverWriteVM, src_pid,  src_addr,
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
    readvm(static_cast<uint32_t>(_processid), address, uintptr_t(buffer), size);
  }

  template <typename T>
  void write(const uintptr_t address, const T& buffer, int Size) {
    writevm(_processid, address, (uintptr_t)&buffer, Size);
  }
  auto dll_address(const char* dllname) -> ULONG64 {
    Requests out = {0};
    out.request_key = kDriverFetchModuleBase;
    out.src_pid = _processid;

    size_t originalLen = strlen(dllname);
    if (originalLen > 32) originalLen = 32;
    out.dll_name_length = originalLen;
    FixedStr64 fs;
    encodeFixedStr64(dllname, &fs);
    out.dll_name = fs;

    DeviceIoControl(_driver_handle, ioctl_call_driver, &out, sizeof(out), &out,
                    sizeof(out), nullptr, nullptr);

    return out.kDriverFetchModuleBase;
  }

  void mouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData,
                   ULONG_PTR dwExtraInfo) {
    Requests request = {0};
    request.request_key = kDriverEmulateHID;
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
#endif
