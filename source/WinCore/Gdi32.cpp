// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

HDC GetDC(HWND hWnd)
{
	return MainApplication().GetDCImpl(hWnd);
}

int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd)
{
	return MainApplication().ChoosePixelFormatImpl(hdc, ppfd);
}

BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd)
{
	return MainApplication().SetPixelFormatImpl(hdc, format, ppfd);
}

HGLRC wglCreateContext(HDC hdc)
{
	return MainApplication().wglCreateContextImpl(hdc);
}

BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc)
{
	return MainApplication().wglMakeCurrentImpl(hdc, hglrc);
}

BOOL SwapBuffers(HDC hdc)
{
	return MainApplication().SwapBuffers(hdc);
}

int SetDIBitsToDevice(HDC hdc, int xDest, int yDest, DWORD wDest, DWORD hDest, int xSrc, int ySrc, UINT uStartScan, UINT cScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT colorUse)
{
	return MainApplication().SetDIBitsToDeviceImpl(hdc, xDest, yDest, wDest, hDest, xSrc, ySrc, uStartScan, cScanLines, lpvBits, lpbmi, colorUse);
}

int StretchDIBits(HDC hdc, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop)
{
	return MainApplication().StretchDIBitsImpl(hdc, xDest, yDest, wDest, hDest, xSrc, ySrc, wSrc, hSrc, lpBits, lpbmi, iUsage, rop);
}

int ReleaseDC(HWND hWnd, HDC hDC)
{
	return MainApplication().ReleaseDCImpl(hWnd, hDC);
}
