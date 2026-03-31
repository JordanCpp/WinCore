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

#include <WinCore/Application.hpp>

static Application _application;

Application::Application()
{
}

Application::~Application()
{
}

ATOM Application::RegisterClassImpl(const WNDCLASSA* wndClass)
{
	_classRegistrator.Append(wndClass);

	return true;
}

HWND Application::CreateWindowExAImpl(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	Window* window = _windowCreator.Create(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	_windowManager.Append((HWND)window, window);

	WindowClassA windowClass;

	if (_classRegistrator.Find(lpClassName, windowClass))
	{
		windowClass.lpfnWndProc((HWND)window, WM_CREATE, 0, 0);
	}

	return (HWND)window;
}

BOOL Application::GetMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	MSG msg = { 0 };

	if (_eventHandler.IsRunning())
	{
		if (_eventHandler.GetEvent(msg))
		{
			for (ClassRegistrator::container::const_iterator i = _classRegistrator.GetClasses().begin(); i != _classRegistrator.GetClasses().end(); i++)
			{
				i->second.lpfnWndProc(NULL, msg.message, msg.wParam, msg.lParam);
			}
		}
	}

	return _eventHandler.IsRunning();
}

void Application::PostQuitMessageImpl(int nExitCode)
{
	_eventHandler.StopEvents();
}

LRESULT Application::DefWindowProcAImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

Application& MainApplication()
{
	return _application;
}
