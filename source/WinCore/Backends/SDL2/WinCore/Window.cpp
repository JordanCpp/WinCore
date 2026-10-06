// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL_video.h>
#include <SDL_render.h>
#include <WinCore/Window.hpp>

static Uint32 StylesToFlags(DWORD dwStyle, DWORD dwExStyle, bool forceOpenGL = false)
{
    Uint32 flags = 0;

    if (dwStyle & WS_POPUP)
    {
        flags |= SDL_WINDOW_BORDERLESS;
    }

    if (dwStyle & WS_THICKFRAME)
    {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    if (!(dwStyle & WS_VISIBLE))
    {
        flags |= SDL_WINDOW_HIDDEN;
    }

    if (dwStyle & WS_MINIMIZE)
    {
        flags |= SDL_WINDOW_MINIMIZED;
    }
    else if (dwStyle & WS_MAXIMIZE)
    {
        flags |= SDL_WINDOW_MAXIMIZED;
    }

    if (dwExStyle & WS_EX_TOPMOST)
    {
        flags |= SDL_WINDOW_ALWAYS_ON_TOP;
    }

    if (dwExStyle & WS_EX_TOOLWINDOW)
    {
        flags |= SDL_WINDOW_UTILITY;
    }

    if (forceOpenGL)
    {
        flags |= SDL_WINDOW_OPENGL;
    }

    return flags;
}

Window::Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) :
    _window(NULL),
    _renderer(NULL),
    _glContext(NULL),
    _paintValid(TRUE)
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

    Uint32 flags = StylesToFlags(_baseWindow.dwStyle, _baseWindow.dwExStyle);

    _window = SDL_CreateWindow(_baseWindow.lpWindowName.c_str(), x, y, w, h, flags);
}

Window::~Window()
{
    if (_glContext)
    {
        SDL_GL_DeleteContext(_glContext);
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

    int x = 0, y = 0, w = 0, h = 0;

    SDL_GetWindowPosition(_window, &x, &y);
    SDL_GetWindowSize(_window, &w, &h);

    bool isCurrentlyVisible = (_window && !(SDL_GetWindowFlags(_window) & SDL_WINDOW_HIDDEN));

    if (_window)
    {
        SDL_DestroyWindow(_window);
    }

    DWORD activeStyle = _baseWindow.dwStyle;
    if (isCurrentlyVisible)
    {
        activeStyle |= WS_VISIBLE;
    }

    Uint32 flags = StylesToFlags(activeStyle, _baseWindow.dwExStyle, true);
    flags |= SDL_WINDOW_OPENGL;

    _window = SDL_CreateWindow(_baseWindow.lpWindowName.c_str(), x, y, w, h, flags);

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
        SDL_GL_SwapWindow(_window);
        return TRUE;
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
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

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

    if (SDL_UpdateTexture(texture, NULL, lpBits, pitch) != 0)
    {
        SDL_DestroyTexture(texture);
        return FALSE;
    }

    SDL_Rect srcRect;
    srcRect.x = xSrc;
    srcRect.y = ySrc;
    srcRect.w = wSrc;
    srcRect.h = hSrc;

    SDL_Rect destRect;
    destRect.x = xDest;
    destRect.y = yDest;
    destRect.w = wDest;
    destRect.h = hDest;

    SDL_RenderClear(_renderer);

    if (biHeight > 0)
    {
        SDL_RenderCopyEx(_renderer, texture, &srcRect, &destRect, 0.0, NULL, SDL_FLIP_VERTICAL);
    }
    else
    {
        SDL_RenderCopy(_renderer, texture, &srcRect, &destRect);
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

    int x = 0, y = 0, w = 0, h = 0;

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
    _paintValid = valid;
}

BOOL Window::IsPaintValid() const
{
    return _paintValid;
}
