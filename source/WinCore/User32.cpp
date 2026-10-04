// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

ATOM RegisterClassA(const WNDCLASSA* lpWndClass)
{
	MainApplication().RegisterClassImpl(lpWndClass);

	return true;
}

HWND CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND result = NULL;

	result = MainApplication().CreateWindowExAImpl(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	return result;
}

BOOL GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	return MainApplication().GetMessageAImpl(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

BOOL PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	return MainApplication().PeekMessageAImpl(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

void PostQuitMessage(int nExitCode)
{
	MainApplication().PostQuitMessageImpl(nExitCode);
}

LRESULT DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return MainApplication().DefWindowProcAImpl(hWnd, Msg, wParam, lParam);
}

LRESULT DispatchMessageA(const MSG* lpMsg)
{
	return MainApplication().DispatchMessageA(lpMsg);
}

HMODULE GetModuleHandleA(LPCSTR lpModuleName)
{
	return 0;
}

HBRUSH GetSysColorBrush(int nIndex)
{
	return 0;
}
