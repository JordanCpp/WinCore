// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Window.hpp>

Window::Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) :
	_window(NULL),
	_glContext(NULL)
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

	int x = (_baseWindow.X == CW_USEDEFAULT) ? SDL_WINDOWPOS_UNDEFINED : _baseWindow.X;
	int y = (_baseWindow.Y == CW_USEDEFAULT) ? SDL_WINDOWPOS_UNDEFINED : _baseWindow.Y;
	int w = (_baseWindow.nWidth  == CW_USEDEFAULT) ? 800 : _baseWindow.nWidth;
	int h = (_baseWindow.nHeight == CW_USEDEFAULT) ? 600 : _baseWindow.nHeight;

	_window = SDL_CreateWindow(_baseWindow.lpWindowName.c_str(), x, y, w, h, SDL_WINDOW_OPENGL);
}

Window::~Window()
{
	if (_glContext)
	{
		SDL_GL_DeleteContext(_glContext);
	}

	if (_window)
	{
		SDL_DestroyWindow(_window);
	}
}

void* Window::Native()
{
	return _window;
}

void Window::CreateContext()
{
	_glContext = SDL_GL_CreateContext(_window);
}

BOOL Window::MakeCurrent()
{
	return false;
}

BOOL Window::SwapBuffers()
{
	SDL_GL_SwapWindow(_window);

	return true;
}
