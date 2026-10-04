// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL2_Window_hpp
#define WinCore_SDL2_Window_hpp

#include <SDL.h>
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
	SDL_GLContext  _glContext;
	BaseWindow     _baseWindow;
};

#endif
