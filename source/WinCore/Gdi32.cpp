/*
Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp)

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, see <https://gnu.org>.
*/

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