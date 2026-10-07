// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Window.hpp>

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x00000002

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
		return TRUE;
	}

	return FALSE;
}

BOOL Window::SwapBuffers()
{
	if (_glContext)
	{
		return SDL_GL_SwapWindow(_window);
	}

	return FALSE;
}

BOOL Window::BlitDIBits(int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, int srcWidth, int srcHeight, int biHeight)
{
	if (_glContext)
	{
		return FALSE;
	}

	if (!_renderer)
	{
		_renderer = SDL_CreateRenderer(_window, NULL);

		if (!_renderer)
		{
			return FALSE;
		}
	}

	SDL_Texture* texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, srcWidth, srcHeight);
	if (!texture) 
	{
		return FALSE;
	}

	int pitch = srcWidth * 4;

	if (!SDL_UpdateTexture(texture, NULL, lpBits, pitch))
	{
		SDL_DestroyTexture(texture);

		return FALSE;
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

	return TRUE;
}

BOOL Window::GetClientRect(LPRECT lpRect)
{
	if (!lpRect || !_window)
	{
		return FALSE;
	}

	lpRect->left = 0;
	lpRect->top = 0;
	lpRect->right = static_cast<LONG>(GetWidth());
	lpRect->bottom = static_cast<LONG>(GetHeight());

	return TRUE;
}

BOOL Window::ShowWindow(int nCmdShow)
{
	if (!_window) return FALSE;

	if (nCmdShow == SW_HIDE) 
	{
		SDL_HideWindow(_window);
		_baseWindow.dwStyle &= ~WS_VISIBLE;
		return FALSE;
	}
	else
	{
		SDL_ShowWindow(_window);
		_baseWindow.dwStyle |= WS_VISIBLE;
		return FALSE;
	}
}

BOOL Window::UpdateWindow()
{
	if (!_window)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL Window::GetWindowRect(LPRECT lpRect)
{
	if (!lpRect || !_window)
	{
		return FALSE;
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

	return TRUE;
}

BOOL Window::SetWindowTextA(LPCSTR lpString)
{
	if (!_window)
	{
		return FALSE;
	}

	_baseWindow.lpWindowName = lpString ? lpString : "";

	SDL_SetWindowTitle(_window, _baseWindow.lpWindowName.c_str());

	return TRUE;
}

int Window::GetWidth() const
{
	if (!_window) return 0;
	int w = 0, h = 0;
	SDL_GetWindowSizeInPixels(_window, &w, &h);

	return w;
}

int Window::GetHeight() const
{
	if (!_window) return 0;
	int w = 0, h = 0;
	SDL_GetWindowSizeInPixels(_window, &w, &h);

	return h;
}

void Window::SetPaintValid(BOOL valid)
{
	_baseWindow.SetPaintValid(valid);
}

BOOL Window::IsPaintValid() const
{
	return _baseWindow.IsPaintValid();
}

int Window::ChoosePixelFormat(const PIXELFORMATDESCRIPTOR* ppfd)
{
	if (!ppfd)
	{
		return 0;
	}

	if (!(ppfd->dwFlags & PFD_SUPPORT_OPENGL))
	{
		return 0;
	}

	if (ppfd->dwFlags & PFD_DOUBLEBUFFER)
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
	}

	if (ppfd->cColorBits >= 24)
	{
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		if (ppfd->cColorBits == 32)
		{
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		}
		else
		{
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
		}
	}
	else if (ppfd->cColorBits == 16)
	{
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
	}

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, ppfd->cDepthBits);

	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, ppfd->cStencilBits);

	return 1;
}

BOOL Window::SetPixelFormat(int format, const PIXELFORMATDESCRIPTOR* ppfd)
{
	if (format != 1)
	{
		return FALSE;
	}

	if (_glContext)
	{
		return FALSE;
	}

	if (ppfd)
	{
		if (!(ppfd->dwFlags & PFD_SUPPORT_OPENGL))
		{
			return FALSE;
		}

		if (ppfd->dwFlags & PFD_DOUBLEBUFFER)
		{
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		}
		else
		{
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
		}

		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, ppfd->cDepthBits);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, ppfd->cStencilBits);
	}

	return TRUE;
}

HGLRC Window::wglCreateContextAttribsARB(HGLRC hShareContext, const int* attribList)
{
	int major = 1;
	int minor = 0;
	int profileMask = 0;
	int contextFlags = 0;

	if (attribList)
	{
		for (int i = 0; attribList[i] != 0; i += 2)
		{
			switch (attribList[i])
			{
			case WGL_CONTEXT_MAJOR_VERSION_ARB:
				major = attribList[i + 1];
				break;
			case WGL_CONTEXT_MINOR_VERSION_ARB:
				minor = attribList[i + 1];
				break;
			case WGL_CONTEXT_PROFILE_MASK_ARB:
				profileMask = attribList[i + 1];
				break;
			case WGL_CONTEXT_FLAGS_ARB:
				contextFlags = attribList[i + 1];
				break;
			default:
				break;
			}
		}
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

	if (profileMask & WGL_CONTEXT_CORE_PROFILE_BIT_ARB)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	}
	else if (profileMask & WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
	}

	int sdlContextFlags = 0;
	if (contextFlags & WGL_CONTEXT_DEBUG_BIT_ARB)
	{
		sdlContextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
	}
	if (contextFlags & WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB)
	{
		sdlContextFlags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, sdlContextFlags);

	CreateContext();

	return (HGLRC)this;
}