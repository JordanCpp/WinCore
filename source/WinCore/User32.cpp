// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

ATOM RegisterClassA(const WNDCLASSA* lpWndClass)
{
	if (!lpWndClass) return 0;

	MainApplication()._classRegistrator.Append(lpWndClass);

	return true;
}

HWND CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	Window* window = MainApplication()._windowCreator.Create(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (!window) return NULL;

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

	MSG msg = { 0 };

	if (MainApplication()._eventHandler.IsRunning())
	{
		if (MainApplication()._eventHandler.WaitEvent(msg))
		{
			lpMsg->hwnd = msg.hwnd;
			lpMsg->message = msg.message;
			lpMsg->wParam = msg.wParam;
			lpMsg->lParam = msg.lParam;
			lpMsg->time = msg.time;
			lpMsg->pt = msg.pt;

			if (msg.message == WM_QUIT)
			{
				MainApplication()._eventHandler.StopEvents();

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	if (!lpMsg)
	{
		return FALSE;
	}

	MSG msg = { 0 };
	bool bRemove = (wRemoveMsg == PM_REMOVE);

	if (MainApplication()._eventHandler.GetEvent(msg, bRemove))
	{
		lpMsg->hwnd = msg.hwnd;
		lpMsg->message = msg.message;
		lpMsg->wParam = msg.wParam;
		lpMsg->lParam = msg.lParam;
		lpMsg->time = msg.time;
		lpMsg->pt = msg.pt;

		if (msg.message == WM_QUIT && bRemove)
		{
			MainApplication()._eventHandler.StopEvents();
		}

		return TRUE;
	}

	return FALSE;
}

void PostQuitMessage(int nExitCode)
{
	MSG quitMsg = { 0 };
	quitMsg.message = WM_QUIT;
	quitMsg.wParam = (WPARAM)nExitCode;

	MainApplication()._eventHandler.PushMessage(quitMsg);
}

LRESULT DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_CLOSE)
	{
		MainApplication()._eventHandler.StopEvents();
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
	}
	else
	{
		for (ClassRegistrator::container::const_iterator i = MainApplication()._classRegistrator.GetClasses().begin(); i != MainApplication()._classRegistrator.GetClasses().end(); i++)
		{
			i->second.lpfnWndProc(NULL, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
		}
	}

	return true;
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
