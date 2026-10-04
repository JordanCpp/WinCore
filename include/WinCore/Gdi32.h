// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Gdi32_h
#define WinCore_Gdi32_h

#include <WinCore/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PIXELFORMATDESCRIPTOR
{
    WORD  nSize;
    WORD  nVersion;
    DWORD dwFlags;
    BYTE  iPixelType;
    BYTE  cColorBits;
    BYTE  cRedBits;
    BYTE  cRedShift;
    BYTE  cGreenBits;
    BYTE  cGreenShift;
    BYTE  cBlueBits;
    BYTE  cBlueShift;
    BYTE  cAlphaBits;
    BYTE  cAlphaShift;
    BYTE  cAccumBits;
    BYTE  cAccumRedBits;
    BYTE  cAccumGreenBits;
    BYTE  cAccumBlueBits;
    BYTE  cAccumAlphaBits;
    BYTE  cDepthBits;
    BYTE  cStencilBits;
    BYTE  cAuxBuffers;
    BYTE  iLayerType;
    BYTE  bReserved;
    DWORD dwLayerMask;
    DWORD dwVisibleMask;
    DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR, * PPIXELFORMATDESCRIPTOR, FAR* LPPIXELFORMATDESCRIPTOR;

HDC GetDC(HWND hWnd);
int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd);
BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd);
HGLRC wglCreateContext(HDC hdc);
BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc);
BOOL SwapBuffers(HDC hdc);

#ifdef __cplusplus
}
#endif

#endif
