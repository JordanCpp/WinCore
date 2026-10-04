// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

HDC GetDC(HWND hWnd)
{
	return MainApplication().GetDCImpl(hWnd);
}

int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd)
{
	return 0;
}

BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd)
{
	return 0;
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