// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_Window_hpp
#define WinCore_SDL3_Window_hpp

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <WinCore/BaseWindow.hpp>

class Window
{
public:
	Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	~Window();
	void* Native();
	void CreateContext();
	BOOL MakeCurrent();
	BOOL SwapBuffers();
private:
	SDL_Window*    _window;
	SDL_Renderer*  _renderer;
	SDL_GLContext  _glContext;
	BaseWindow     _baseWindow;
};

#endif
