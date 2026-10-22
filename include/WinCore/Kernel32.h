/* Copyright(C) 2026 Evgeny Zoshchuk(JordanCpp).Licensed under LGPL - 3.0 - or -later. */

#ifndef WinCore_Kernel32_h
#define WinCore_Kernel32_h

#ifdef __cplusplus
extern "C" {
#endif

#include <WinCore/Config.h>
#include <WinCore/Types.h>

#define ERROR_SUCCESS               0
#define ERROR_FILE_NOT_FOUND        2
#define ERROR_PATH_NOT_FOUND        3
#define ERROR_ACCESS_DENIED         5
#define ERROR_INVALID_HANDLE        6
#define ERROR_NOT_ENOUGH_MEMORY     8
#define ERROR_INVALID_PARAMETER    87
#define ERROR_MOD_NOT_FOUND        126
#define ERROR_PROC_NOT_FOUND       127
#define ERROR_CLASS_ALREADY_EXISTS 1410
#define ERROR_NOT_SUPPORTED 50L

typedef int (FAR WINAPI* FARPROC)();

WINCORE_API DWORD GetLastError(void);

WINCORE_API void SetLastError(DWORD dwErrCode);

WINCORE_API HMODULE LoadLibraryA(LPCSTR lpLibFileName);

WINCORE_API BOOL FreeLibrary(HMODULE hLibModule);

WINCORE_API FARPROC GetProcAddress(HMODULE hModule, LPCSTR  lpProcName);

WINCORE_API DWORD GetTickCount();

WINCORE_API void Sleep(DWORD dwMilliseconds);

#ifdef __cplusplus
}
#endif

#endif
