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

#ifndef WinCore_User32_h
#define WinCore_User32_h

#ifdef __cplusplus
extern "C" {
#endif

#include <WinCore/Types.h>

#define WM_CREATE   0x0001
#define WM_DESTROY  0x0002

#define CS_VREDRAW  0x0001
#define CS_HREDRAW  0x0002

typedef struct POINT
{
    LONG  x;
    LONG  y;
} POINT;

typedef struct MSG
{
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    POINT       pt;
} MSG, * PMSG, NEAR* NPMSG, FAR* LPMSG;

typedef struct WNDCLASSA 
{
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
} WNDCLASSA;

#ifdef UNICODE
#else
    typedef WNDCLASSA WNDCLASS;
#endif


HMODULE GetModuleHandleA(LPCSTR lpModuleName);

HBRUSH GetSysColorBrush(int nIndex);

ATOM RegisterClassA(const WNDCLASSA* lpWndClass);

HWND CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

BOOL GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);

void PostQuitMessage(int nExitCode);

LRESULT DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

LRESULT DispatchMessageA(const MSG* lpMsg);

#ifdef UNICODE
#else
    #define RegisterClass    RegisterClassA
    #define CreateWindowEx   CreateWindowExA 
    #define GetMessage       GetMessageA
    #define DefWindowProc    DefWindowProcA
    #define DispatchMessage  DispatchMessageA
#endif

#ifdef __cplusplus
}
#endif

#endif
