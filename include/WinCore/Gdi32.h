// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Gdi32_h
#define WinCore_Gdi32_h

#include <WinCore/Config.h>
#include <WinCore/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PFD_DOUBLEBUFFER            0x00000001
#define PFD_STEREO                  0x00000002
#define PFD_DRAW_TO_WINDOW          0x00000004
#define PFD_DRAW_TO_BITMAP          0x00000008
#define PFD_SUPPORT_GDI             0x00000010
#define PFD_SUPPORT_OPENGL          0x00000020
#define PFD_GENERIC_FORMAT          0x00000040
#define PFD_NEED_PALETTE            0x00000080
#define PFD_NEED_SYSTEM_PALETTE     0x00000100
#define PFD_SWAP_EXCHANGE           0x00000200
#define PFD_SWAP_COPY               0x00000400
#define PFD_SWAP_LAYER_BUFFERS      0x00000800
#define PFD_GENERIC_ACCELERATED     0x00001000
#define PFD_SUPPORT_DIRECTDRAW      0x00002000

#define PFD_TYPE_RGBA               0
#define PFD_TYPE_COLORINDEX         1

#define PFD_MAIN_PLANE              0
#define PFD_OVERLAY_PLANE           1
#define PFD_UNDERLAY_PLANE          (-1)

    typedef struct tagPIXELFORMATDESCRIPTOR 
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
    } PIXELFORMATDESCRIPTOR, *PPIXELFORMATDESCRIPTOR, *LPPIXELFORMATDESCRIPTOR;

WINCORE_API HDC GetDC(HWND hWnd);
WINCORE_API int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd);
WINCORE_API BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd);
WINCORE_API HGLRC wglCreateContext(HDC hdc);
WINCORE_API BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc);
WINCORE_API BOOL SwapBuffers(HDC hdc);

#ifdef __cplusplus
}
#endif

#endif
