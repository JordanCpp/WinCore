/* Copyright(C) 2026 Evgeny Zoshchuk(JordanCpp).Licensed under LGPL - 3.0 - or -later. */

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

#pragma pack(push, 1)

    typedef struct BITMAPINFOHEADER
    {
        DWORD biSize;
        LONG  biWidth;
        LONG  biHeight;
        WORD  biPlanes;
        WORD  biBitCount;
        DWORD biCompression;
        DWORD biSizeImage;
        LONG  biXPelsPerMeter;
        LONG  biYPelsPerMeter;
        DWORD biClrUsed;
        DWORD biClrImportant;
    } BITMAPINFOHEADER;

    typedef struct RGBQUAD
    {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } RGBQUAD;

    typedef struct BITMAPINFO
    {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD          bmiColors[1];
    } BITMAPINFO;

#pragma pack(pop)

#define BI_RGB 0L
#define DIB_RGB_COLORS 0
#define SRCCOPY (DWORD)0x00CC0020

WINCORE_API HDC GetDC(HWND hWnd);
WINCORE_API int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd);
WINCORE_API BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd);
WINCORE_API HGLRC wglCreateContext(HDC hdc);
WINCORE_API BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc);
WINCORE_API BOOL SwapBuffers(HDC hdc);

WINCORE_API int SetDIBitsToDevice(HDC hdc, int xDest, int yDest, DWORD wDest, DWORD hDest, int xSrc, int ySrc, UINT uStartScan, UINT cScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT colorUse);
WINCORE_API int StretchDIBits(HDC hdc, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop);
WINCORE_API int ReleaseDC(HWND hWnd, HDC hDC);

#ifdef __cplusplus
}
#endif

#endif
