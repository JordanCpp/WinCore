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

#include <WinCore/Window.hpp>

Window::Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	_baseWindow.dwExStyle    = dwExStyle;
	_baseWindow.lpClassName  = lpClassName;
	_baseWindow.lpWindowName = lpWindowName;
	_baseWindow.dwStyle      = dwStyle;
	_baseWindow.X            = X;
	_baseWindow.Y            = Y;
	_baseWindow.nWidth       = nWidth;
	_baseWindow.nHeight      = nHeight;
	_baseWindow.hWndParent   = hWndParent;
	_baseWindow.hMenu        = hMenu;
	_baseWindow.hInstance    = hInstance;
	_baseWindow.lpParam      = lpParam;
}

Window::~Window()
{
}

void* Window::Native()
{
	return NULL;
}
