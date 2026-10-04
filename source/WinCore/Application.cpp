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
	if (!wndClass) return 0;

	_classRegistrator.Append(wndClass);
	return true;
}

HWND Application::CreateWindowExAImpl(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	Window* window = _windowCreator.Create(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (!window) return NULL;

	HWND hFakeWnd = (HWND)window;

	_windowManager.Append(hFakeWnd, window);

	WindowClassA windowClass;

	if (_classRegistrator.Find(lpClassName, windowClass))
	{
		CREATESTRUCTA cs;
		cs.lpCreateParams = lpParam;
		cs.hInstance      = hInstance;
		cs.hMenu          = hMenu;
		cs.hwndParent     = hWndParent;
		cs.cy             = nHeight;
		cs.cx             = nWidth;
		cs.y              = Y;
		cs.x              = X;
		cs.style          = dwStyle;
		cs.lpszName       = lpWindowName;
		cs.lpszClass      = lpClassName;
		cs.dwExStyle      = dwExStyle;

		windowClass.lpfnWndProc(hFakeWnd, WM_CREATE, 0, (LPARAM)&cs);
	}

	return hFakeWnd;
}

BOOL Application::GetMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	if (!lpMsg)
	{
		return false;
	}

	MSG msg = { 0 };

	if (_eventHandler.IsRunning())
	{
		if (_eventHandler.WaitEvent(msg))
		{
			lpMsg->hwnd    = msg.hwnd;
			lpMsg->message = msg.message;
			lpMsg->wParam  = msg.wParam;
			lpMsg->lParam  = msg.lParam;
			lpMsg->time    = msg.time;
			lpMsg->pt      = msg.pt;

			if (msg.message == WM_QUIT)
			{
				_eventHandler.StopEvents();

				return false;
			}

			return true;
		}
	}

	return false;
}

BOOL Application::PeekMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	if (!lpMsg) return false;

	MSG msg = { 0 };
	bool bRemove = (wRemoveMsg == PM_REMOVE);

	if (_eventHandler.GetEvent(msg, bRemove))
	{
		lpMsg->hwnd = msg.hwnd;
		lpMsg->message = msg.message;
		lpMsg->wParam = msg.wParam;
		lpMsg->lParam = msg.lParam;
		lpMsg->time = msg.time;
		lpMsg->pt = msg.pt;

		if (msg.message == WM_QUIT && bRemove)
		{
			_eventHandler.StopEvents();
		}

		return true;
	}

	return false;
}

void Application::PostQuitMessageImpl(int nExitCode)
{
	MSG quitMsg     = { 0 };
	quitMsg.message = WM_QUIT;
	quitMsg.wParam  = (WPARAM)nExitCode;

	_eventHandler.PushMessage(quitMsg);
}

LRESULT Application::DefWindowProcAImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_CLOSE)
	{
		_eventHandler.StopEvents();
	}

	return 0;
}

LRESULT Application::DispatchMessageAImpl(const MSG* lpMsg)
{
	if (!lpMsg)
	{

		return 0;
	}

	if (lpMsg->hwnd)
	{
		Window* window = _windowManager.Find(lpMsg->hwnd);

		if (window)
		{
			WindowClassA windowClass;

			if (_classRegistrator.Find(window->GetClassName(), windowClass))
			{
				return windowClass.lpfnWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
			}
		}
	}
	else
	{
		for (ClassRegistrator::container::const_iterator i = _classRegistrator.GetClasses().begin(); i != _classRegistrator.GetClasses().end(); i++)
		{
			i->second.lpfnWndProc(NULL, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
		}
	}

	return true;
}

BOOL Application::TranslateMessageImpl(const MSG* lpMsg)
{
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
		return window->MakeCurrent();
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
