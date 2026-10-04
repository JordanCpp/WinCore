// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Kernel32_h
#define WinCore_Kernel32_h

#ifdef __cplusplus
extern "C" {
#endif

#include <WinCore/Types.h>

typedef int (FAR WINAPI* FARPROC)();

HMODULE LoadLibraryA(LPCSTR lpLibFileName);

BOOL FreeLibrary(HMODULE hLibModule);

FARPROC GetProcAddress(HMODULE hModule, LPCSTR  lpProcName);

#ifdef __cplusplus
}
#endif

#endif
