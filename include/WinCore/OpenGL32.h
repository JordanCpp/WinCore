/* Copyright(C) 2026 Evgeny Zoshchuk(JordanCpp).Licensed under LGPL - 3.0 - or -later. */

#ifndef WinCore_OpenGL32_h
#define WinCore_OpenGL32_h

#ifdef __cplusplus
extern "C" {
#endif

#include <WinCore/Config.h>
#include <WinCore/Types.h>

WINCORE_API PROC wglGetProcAddress(LPCSTR unnamedParam1);
WINCORE_API BOOL wglDeleteContext(HGLRC hglrc);

#ifdef __cplusplus
}
#endif

#endif
