// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Window.hpp>

Window::Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) :
	_window(NULL),
	_renderer(NULL),
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

	_window   = SDL_CreateWindow(_baseWindow.lpWindowName.c_str(), _baseWindow.nWidth, _baseWindow.nHeight, SDL_WINDOW_OPENGL);
	_renderer = SDL_CreateRenderer(_window, _baseWindow.lpClassName.c_str());
}

Window::~Window()
{
	if (_glContext)
	{
		SDL_GL_DestroyContext(_glContext);
	}

	if (_renderer)
	{
		SDL_DestroyRenderer(_renderer);
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
	if (SDL_GL_MakeCurrent(_window, _glContext) == 0)
	{
		return true;
	}

	return false;
}

BOOL Window::SwapBuffers()
{
	return SDL_GL_SwapWindow(_window);
}
