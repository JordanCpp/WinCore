// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Window.hpp>

static void StylesToProperties(SDL_PropertiesID props, DWORD dwStyle, DWORD dwExStyle)
{
	if (dwStyle & WS_POPUP)
	{
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
	}

	if (dwStyle & WS_THICKFRAME)
	{
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
	}

	if (!(dwStyle & WS_VISIBLE))
	{
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
	}

	if (dwStyle & WS_MINIMIZE)
	{
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN, true);
	}
	else if (dwStyle & WS_MAXIMIZE)
	{
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, true);
	}

	if (dwExStyle & WS_EX_TOPMOST)
	{
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true);
	}

	if (dwExStyle & WS_EX_TOOLWINDOW)
	{
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN, true);
	}
}

Window::Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) :
	_window(NULL),
	_renderer(NULL),
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

	int w = (_baseWindow.nWidth == CW_USEDEFAULT) ? 800 : _baseWindow.nWidth;
	int h = (_baseWindow.nHeight == CW_USEDEFAULT) ? 600 : _baseWindow.nHeight;

	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, _baseWindow.lpWindowName.c_str());
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, x);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, w);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, h);

	StylesToProperties(props, _baseWindow.dwStyle, _baseWindow.dwExStyle);

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
	if (_glContext)
	{
		return;
	}

	if (_renderer)
	{
		SDL_DestroyRenderer(_renderer);
		_renderer = NULL;
	}

	int x = 0;
	int	y = 0;
	int	w = 0;
	int	h = 0;

	SDL_GetWindowPosition(_window, &x, &y);
	SDL_GetWindowSize(_window, &w, &h);

	bool isCurrentlyVisible = (_window && !(SDL_GetWindowFlags(_window) & SDL_WINDOW_HIDDEN));

	if (_window)
	{
		SDL_DestroyWindow(_window);
	}

	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, _baseWindow.lpWindowName.c_str());
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, x);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, w);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, h);
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);

	DWORD activeStyle = _baseWindow.dwStyle;
	if (isCurrentlyVisible)
	{
		activeStyle |= WS_VISIBLE;
	}

	StylesToProperties(props, activeStyle, _baseWindow.dwExStyle);

	_window = SDL_CreateWindowWithProperties(props);
	SDL_DestroyProperties(props);

	_glContext = SDL_GL_CreateContext(_window);
}

BOOL Window::MakeCurrent()
{
	if (!_glContext)
	{
		CreateContext();
	}

	if (SDL_GL_MakeCurrent(_window, _glContext) == 0)
	{
		return true;
	}

	return false;
}

BOOL Window::SwapBuffers()
{
	if (_glContext)
	{
		return SDL_GL_SwapWindow(_window);
	}

	return false;
}

BOOL Window::BlitDIBits(int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, int srcWidth, int srcHeight, int biHeight)
{
	if (_glContext)
	{
		return false;
	}

	if (!_renderer)
	{
		_renderer = SDL_CreateRenderer(_window, NULL);

		if (!_renderer)
		{
			return false;
		}
	}

	SDL_Texture* texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, srcWidth, srcHeight);
	if (!texture) 
	{
		return false;
	}

	int pitch = srcWidth * 4;

	if (!SDL_UpdateTexture(texture, NULL, lpBits, pitch))
	{
		SDL_DestroyTexture(texture);

		return false;
	}

	SDL_FRect srcRect;
	srcRect.x = static_cast<float>(xSrc);
	srcRect.y = static_cast<float>(ySrc);
	srcRect.w = static_cast<float>(wSrc);
	srcRect.h = static_cast<float>(hSrc);

	SDL_FRect destRect;
	destRect.x = static_cast<float>(xDest);
	destRect.y = static_cast<float>(yDest);
	destRect.w = static_cast<float>(wDest);
	destRect.h = static_cast<float>(hDest);

	SDL_RenderClear(_renderer);

	if (biHeight > 0)
	{
		SDL_RenderTextureRotated(_renderer, texture, &srcRect, &destRect, 0.0, NULL, SDL_FLIP_VERTICAL);
	}
	else
	{
		SDL_RenderTexture(_renderer, texture, &srcRect, &destRect);
	}

	SDL_RenderPresent(_renderer);
	SDL_DestroyTexture(texture);

	return true;
}

BOOL Window::GetClientRectImpl(LPRECT lpRect)
{
	if (!lpRect || !_window)
	{
		return false;
	}

	int w = 0;
	int h = 0;

	SDL_GetWindowSize(_window, &w, &h);

	lpRect->left   = 0;
	lpRect->top    = 0;
	lpRect->right  = static_cast<LONG>(w);
	lpRect->bottom = static_cast<LONG>(h);

	return true;
}

BOOL Window::ShowWindow(int nCmdShow)
{
	if (!_window) return false;

	if (nCmdShow == SW_HIDE) 
	{
		SDL_HideWindow(_window);
		_baseWindow.dwStyle &= ~WS_VISIBLE;
		return false;
	}
	else
	{
		SDL_ShowWindow(_window);
		_baseWindow.dwStyle |= WS_VISIBLE;
		return true;
	}
}

BOOL Window::UpdateWindow()
{
	if (!_window)
	{
		return false;
	}

	return true;
}

BOOL Window::GetWindowRect(LPRECT lpRect)
{
	if (!lpRect || !_window)
	{
		return false;
	}

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	SDL_GetWindowPosition(_window, &x, &y);

	SDL_GetWindowSize(_window, &w, &h);

	lpRect->left = static_cast<LONG>(x);
	lpRect->top = static_cast<LONG>(y);
	lpRect->right = static_cast<LONG>(x + w);
	lpRect->bottom = static_cast<LONG>(y + h);

	return true;
}
