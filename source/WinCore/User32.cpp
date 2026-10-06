// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

ATOM RegisterClassA(const WNDCLASSA* lpWndClass)
{
	if (!lpWndClass)
	{
		return 0;
	}

	MainApplication()._classRegistrator.Append(lpWndClass);

	return true;
}

HWND CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	Window* window = MainApplication()._windowCreator.Create(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	
	if (!window)
	{
		return NULL;
	}

	HWND hFakeWnd = (HWND)window;

	MainApplication()._windowManager.Append(hFakeWnd, window);

	WindowClassA windowClass;

	if (MainApplication()._classRegistrator.Find(lpClassName, windowClass))
	{
		CREATESTRUCTA cs;
		cs.lpCreateParams = lpParam;
		cs.hInstance = hInstance;
		cs.hMenu = hMenu;
		cs.hwndParent = hWndParent;
		cs.cy = nHeight;
		cs.cx = nWidth;
		cs.y = Y;
		cs.x = X;
		cs.style = dwStyle;
		cs.lpszName = lpWindowName;
		cs.lpszClass = lpClassName;
		cs.dwExStyle = dwExStyle;

		windowClass.lpfnWndProc(hFakeWnd, WM_CREATE, 0, (LPARAM)&cs);
	}

	return hFakeWnd;
}

BOOL DestroyWindow(HWND hWnd)
{
	if (!hWnd)
	{
		return FALSE;
	}

	Window* window = MainApplication()._windowManager.Find(hWnd);

	if (!window)
	{
		return FALSE;
	}

	WindowClassA windowClass;

	if (MainApplication()._classRegistrator.Find(window->GetClassName(), windowClass))
	{
		windowClass.lpfnWndProc(hWnd, WM_DESTROY, 0, 0);
		windowClass.lpfnWndProc(hWnd, WM_NCDESTROY, 0, 0);
	}

	return MainApplication()._windowManager.Destroy(hWnd);
}

BOOL GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	if (!lpMsg)
	{
		return FALSE;
	}

	EventHandler& events = MainApplication()._eventHandler;
	MessageQueue& queue  = events.Messages();

	for (;;)
	{
		events.PumpEvents();

		MSG msg;
		bool ok = (hWnd == NULL && wMsgFilterMin == 0 && wMsgFilterMax == 0) ? queue.Pop(msg) : queue.PeekFiltered(msg, true, hWnd, wMsgFilterMin, wMsgFilterMax);

		if (ok)
		{
			*lpMsg = msg;

			if (msg.message == WM_QUIT)
			{
				return FALSE;
			}

			return TRUE;
		}

		if (!queue.IsRunning())
		{
			return FALSE;
		}

		if (!events.WaitAndPush())
		{
			return FALSE;
		}
	}
}

BOOL PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	if (!lpMsg)
	{
		return FALSE;
	}

	EventHandler& events = MainApplication()._eventHandler;
	events.PumpEvents();

	MessageQueue& queue = events.Messages();
	bool bRemove = (wRemoveMsg == PM_REMOVE);

	MSG msg;
	bool ok = (hWnd == NULL && wMsgFilterMin == 0 && wMsgFilterMax == 0) ? queue.Peek(msg, bRemove) : queue.PeekFiltered(msg, bRemove, hWnd, wMsgFilterMin, wMsgFilterMax);

	if (!ok) return FALSE;

	*lpMsg = msg;

	return TRUE;
}

void PostQuitMessage(int nExitCode)
{
	MainApplication()._eventHandler.Messages().PostQuit((WPARAM)nExitCode);
}

LRESULT DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
		if (hWnd)
		{
			DestroyWindow(hWnd);
		}
		
		return 0;
	}

	return 0;
}

LRESULT DispatchMessageA(const MSG* lpMsg)
{
	if (!lpMsg)
	{
		return 0;
	}

	if (lpMsg->hwnd)
	{
		Window* window = MainApplication()._windowManager.Find(lpMsg->hwnd);

		if (window)
		{
			WindowClassA windowClass;

			if (MainApplication()._classRegistrator.Find(window->GetClassName(), windowClass))
			{
				return windowClass.lpfnWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
			}
		}

		return 0;
	}

	const ClassRegistrator::container& classes = MainApplication()._classRegistrator.GetClasses();

	for (ClassRegistrator::container::const_iterator i = classes.begin(); i != classes.end(); ++i)
	{
		i->second.lpfnWndProc(NULL, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
	}

	return 0;
}

BOOL TranslateMessage(const MSG* lpMsg)
{
	return TRUE;
}

HMODULE GetModuleHandleA(LPCSTR lpModuleName)
{
	return 0;
}

HBRUSH GetSysColorBrush(int nIndex)
{
	return 0;
}

BOOL GetClientRect(HWND hWnd, LPRECT lpRect)
{
	if (!hWnd || !lpRect)
	{
		return FALSE;
	}

	Window* window = MainApplication()._windowManager.Find(hWnd);

	if (!window)
	{
		return FALSE;
	}

	return window->GetClientRect(lpRect);
}

BOOL ShowWindow(HWND hWnd, int nCmdShow)
{
	if (!hWnd)
	{
		return FALSE;
	}

	Window* window = MainApplication()._windowManager.Find(hWnd);

	if (!window)
	{
		return FALSE;
	}

	return window->ShowWindow(nCmdShow);
}

BOOL UpdateWindow(HWND hWnd)
{
	if (!hWnd)
	{
		return FALSE;
	}

	Window* window = MainApplication()._windowManager.Find(hWnd);

	if (!window)
	{
		return FALSE;
	}

	return window->UpdateWindow();
}

BOOL GetWindowRect(HWND hWnd, LPRECT lpRect)
{
	if (!hWnd || !lpRect)
	{
		return false;
	}

	Window* window = MainApplication()._windowManager.Find(hWnd);

	if (!window)
	{
		return false;
	}

	return window->GetWindowRect(lpRect);
}

BOOL GetCursorPos(LPPOINT lpPoint)
{
	return MainApplication()._cursor.GetCursorPos(lpPoint);
}

BOOL SetCursorPos(int x, int y)
{
	return MainApplication()._cursor.SetCursorPos(x, y);
}

int ShowCursor(BOOL bShow)
{
	return MainApplication()._cursor.ShowCursor(bShow);
}

SHORT GetAsyncKeyState(int vKey)
{
	return MainApplication()._asyncKey.GetAsyncKeyStateImpl(vKey);
}

BOOL SetWindowTextA(HWND hWnd, LPCSTR lpString)
{
	if (!hWnd)
	{
		return FALSE;
	}

	Window* window = MainApplication()._windowManager.Find(hWnd);

	if (!window)
	{
		return FALSE;
	}

	return window->SetWindowTextA(lpString);
}
