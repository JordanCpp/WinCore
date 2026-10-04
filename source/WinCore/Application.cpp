// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

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
		if (_eventHandler.WaitEvent(msg))
		{
			lpMsg->hwnd    = NULL;
			lpMsg->message = msg.message;
			lpMsg->wParam  = msg.wParam;
			lpMsg->lParam  = msg.lParam;
			lpMsg->time    = msg.time;
			lpMsg->pt      = msg.pt;
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

LRESULT Application::DispatchMessageA(const MSG* lpMsg)
{
	for (ClassRegistrator::container::const_iterator i = _classRegistrator.GetClasses().begin(); i != _classRegistrator.GetClasses().end(); i++)
	{
		i->second.lpfnWndProc(NULL, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
	}

	return true;
}

HDC Application::GetDCImpl(HWND hWnd)
{
	return (HDC)hWnd;
}

HGLRC Application::wglCreateContextImpl(HDC hdc)
{
	Window* window = _windowManager.Find((HWND)hdc);

	if (window)
	{
		window->CreateContext();
	}

	return (HGLRC)hdc;
}

BOOL Application::wglMakeCurrentImpl(HDC hdc, HGLRC hglrc)
{
	Window* window = _windowManager.Find((HWND)hdc);

	if (window)
	{
		window->MakeCurrent();
	}

	return false;
}

BOOL Application::SwapBuffers(HDC hdc)
{
	Window* window = _windowManager.Find((HWND)hdc);

	if (window)
	{
		return window->SwapBuffers();
	}

	return false;
}

Application& MainApplication()
{
	return _application;
}
