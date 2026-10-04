// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_User32_h
#define WinCore_User32_h

#ifdef __cplusplus
extern "C" {
#endif

#include <WinCore/Types.h>

typedef struct tagCREATESTRUCTA 
{
    LPVOID    lpCreateParams; // Pointer to value passed as the last param to CreateWindowEx
    HINSTANCE hInstance;      // Handle to the module that owns the window
    HMENU     hMenu;          // Handle to the menu to be used by the window
    HWND      hwndParent;     // Handle to the parent window
    int       cy;             // Height of the window, in pixels
    int       cx;             // Width of the window, in pixels
    int       y;              // Y-coordinate of the upper-left corner of the window
    int       x;              // X-coordinate of the upper-left corner of the window
    LONG      style;          // Style flags for the window
    LPCSTR    lpszName;       // Name of the window (title string)
    LPCSTR    lpszClass;      // Pointer to a null-terminated string specifying the class name
    DWORD     dwExStyle;      // Extended style flags for the window
} CREATESTRUCTA, * LPCREATESTRUCTA;

#define PM_NOREMOVE 0x0000
#define PM_REMOVE   0x0001

#define CS_VREDRAW  0x0001
#define CS_HREDRAW  0x0002

#define CW_USEDEFAULT ((int)0x80000000)

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

BOOL PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

void PostQuitMessage(int nExitCode);

LRESULT DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

LRESULT DispatchMessageA(const MSG* lpMsg);

BOOL TranslateMessage(const MSG* lpMsg);

#define CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam) CreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)

#ifdef UNICODE
#else
    #define RegisterClass    RegisterClassA
    #define CreateWindow     CreateWindowA 
    #define GetMessage       GetMessageA
    #define PeekMessage      PeekMessageA
    #define DefWindowProc    DefWindowProcA
    #define DispatchMessage  DispatchMessageA
#endif

#ifdef __cplusplus
}
#endif

#endif
