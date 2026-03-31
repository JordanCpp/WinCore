/*
Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp)

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, see <https://gnu.org>.
*/

#ifndef WinCore_Application_hpp
#define WinCore_Application_hpp

#include <WinCore/ClassRegistrator.hpp>
#include <WinCore/WindowManager.hpp>
#include <WinCore/WindowCreator.hpp>
#include <WinCore/Initializer.hpp>
#include <WinCore/EventHandler.hpp>

class Application
{
public:
	Application();
	~Application();
	ATOM RegisterClassImpl(const WNDCLASSA* wndClass);
	HWND CreateWindowExAImpl(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	BOOL GetMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
	void PostQuitMessageImpl(int nExitCode);
	LRESULT DefWindowProcAImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	Initializer      _initializer;
	EventHandler     _eventHandler;

	ClassRegistrator _classRegistrator;
	WindowCreator    _windowCreator;
	WindowManager    _windowManager;
};

Application& MainApplication();

#endif
