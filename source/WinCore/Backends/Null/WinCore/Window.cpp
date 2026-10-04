// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

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

void Window::CreateContext()
{
}

BOOL Window::MakeCurrent()
{
	return false;
}

BOOL Window::SwapBuffers()
{
	return false;
}
