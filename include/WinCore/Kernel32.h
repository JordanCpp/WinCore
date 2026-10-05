/* Copyright(C) 2026 Evgeny Zoshchuk(JordanCpp).Licensed under LGPL - 3.0 - or -later. */

#ifndef WinCore_Kernel32_h
#define WinCore_Kernel32_h

#ifdef __cplusplus
extern "C" {
#endif

#include <WinCore/Config.h>
#include <WinCore/Types.h>

typedef int (FAR WINAPI* FARPROC)();

WINCORE_API HMODULE LoadLibraryA(LPCSTR lpLibFileName);

WINCORE_API BOOL FreeLibrary(HMODULE hLibModule);

WINCORE_API FARPROC GetProcAddress(HMODULE hModule, LPCSTR  lpProcName);

#ifdef __cplusplus
}
#endif

#endif
