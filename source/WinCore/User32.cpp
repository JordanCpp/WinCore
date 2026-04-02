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
