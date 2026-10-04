// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Null_Window_hpp
#define WinCore_Null_Window_hpp

#include <WinCore/Window.hpp>
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
	BaseWindow _baseWindow;
};

#endif
