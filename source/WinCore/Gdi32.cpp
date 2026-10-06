// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

HDC GetDC(HWND hWnd)
{
	return (HDC)hWnd;
}

int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd)
{
	if (!hdc || !ppfd)
	{
		return 0;
	}

	return 1;
}

BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd)
{
	if (!hdc || format <= 0 || !ppfd) return false;

	Window* window = MainApplication()._windowManager.Find((HWND)hdc);

	if (window)
	{
		return TRUE;
	}

	return FALSE;
}

HGLRC wglCreateContext(HDC hdc)
{
	Window* window = MainApplication()._windowManager.Find((HWND)hdc);

	if (window)
	{
		window->CreateContext();
	}

	return (HGLRC)hdc;
}

BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc)
{
	Window* window = MainApplication()._windowManager.Find((HWND)hdc);

	if (window)
	{
		return window->MakeCurrent();
	}

	return FALSE;
}

BOOL SwapBuffers(HDC hdc)
{
	Window* window = MainApplication()._windowManager.Find((HWND)hdc);

	if (window)
	{
		return window->SwapBuffers();
	}

	return FALSE;
}

int SetDIBitsToDevice(HDC hdc, int xDest, int yDest, DWORD wDest, DWORD hDest, int xSrc, int ySrc, UINT uStartScan, UINT cScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT colorUse)
{
	if (!hdc || !lpvBits || !lpbmi)
	{
		return 0;
	}

	HWND hWnd = (HWND)hdc;
	Window* window = MainApplication()._windowManager.Find(hWnd);
	if (!window)
	{
		return 0;
	}

	int srcWidth = lpbmi->bmiHeader.biWidth;
	int biHeight = lpbmi->bmiHeader.biHeight;
	int srcHeight = (biHeight < 0) ? -biHeight : biHeight;

	BOOL success = window->BlitDIBits(xDest, yDest, static_cast<int>(wDest), static_cast<int>(hDest), xSrc, ySrc, srcWidth, srcHeight, lpvBits, srcWidth, srcHeight, biHeight);

	return success ? cScanLines : 0;
}

int StretchDIBits(HDC hdc, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop)
{
	if (!hdc || !lpBits || !lpbmi)
	{
		return 0;
	}

	HWND hWnd = (HWND)hdc;
	Window* window = MainApplication()._windowManager.Find(hWnd);
	if (!window)
	{
		return 0;
	}

	int srcWidth = lpbmi->bmiHeader.biWidth;
	int biHeight = lpbmi->bmiHeader.biHeight;
	int srcHeight = std::abs(biHeight);

	BOOL success = window->BlitDIBits(xDest, yDest, wDest, hDest, xSrc, ySrc, wSrc, hSrc, lpBits, srcWidth, srcHeight, biHeight);

	return success ? hSrc : 0;
}

int ReleaseDC(HWND hWnd, HDC hDC)
{
	if (!hDC)
	{
		return 0;
	}

	Window* window = MainApplication()._windowManager.Find(hWnd ? hWnd : (HWND)hDC);
	if (!window)
	{
		return 0;
	}

	return 1;
}
