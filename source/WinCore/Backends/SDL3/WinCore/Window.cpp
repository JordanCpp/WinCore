// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Window.hpp>

Window::Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) :
	_window(NULL),
	_glContext(NULL)
{
	_baseWindow.dwExStyle = dwExStyle;
	_baseWindow.lpClassName = lpClassName ? lpClassName : "";
	_baseWindow.lpWindowName = lpWindowName ? lpWindowName : "";
	_baseWindow.dwStyle = dwStyle;
	_baseWindow.X = X;
	_baseWindow.Y = Y;
	_baseWindow.nWidth = nWidth;
	_baseWindow.nHeight = nHeight;
	_baseWindow.hWndParent = hWndParent;
	_baseWindow.hMenu = hMenu;
	_baseWindow.hInstance = hInstance;
	_baseWindow.lpParam = lpParam;

	int x = (_baseWindow.X == CW_USEDEFAULT) ? SDL_WINDOWPOS_UNDEFINED : _baseWindow.X;
	int y = (_baseWindow.Y == CW_USEDEFAULT) ? SDL_WINDOWPOS_UNDEFINED : _baseWindow.Y;

	int w = (_baseWindow.nWidth  == CW_USEDEFAULT) ? 800 : _baseWindow.nWidth;
	int h = (_baseWindow.nHeight == CW_USEDEFAULT) ? 600 : _baseWindow.nHeight;

	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, _baseWindow.lpWindowName.c_str());
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, x);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, w);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, h);
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);

	_window = SDL_CreateWindowWithProperties(props);

	SDL_DestroyProperties(props);
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

const std::string& Window::GetClassName() const
{
	return _baseWindow.lpClassName;
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
