// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL2_Window_hpp
#define WinCore_SDL2_Window_hpp

#include <SDL_video.h>
#include <SDL_render.h>
#include <WinCore/BaseWindow.hpp>

class Window
{
public:
	Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	~Window();
	void* Native();
	const std::string& GetClassName() const;
	void CreateContext();
	BOOL MakeCurrent();
	BOOL SwapBuffers();
	BOOL BlitDIBits(int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, int srcWidth, int srcHeight, int biHeight);
	BOOL GetClientRect(LPRECT lpRect);
	BOOL ShowWindow(int nCmdShow);
	BOOL UpdateWindow();
	BOOL GetWindowRect(LPRECT lpRect);
	BOOL SetWindowTextA(LPCSTR lpString);

	int  GetWidth()  const;
	int  GetHeight() const;

	void SetPaintValid(BOOL valid);
	BOOL IsPaintValid() const;
private:
	SDL_Window*    _window;
	SDL_Renderer*  _renderer;
	SDL_GLContext  _glContext;
	BaseWindow     _baseWindow;
};

#endif
