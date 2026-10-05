/* Copyright(C) 2026 Evgeny Zoshchuk(JordanCpp).Licensed under LGPL - 3.0 - or -later. */

#ifndef WinCore_User32_h
#define WinCore_User32_h

#ifdef __cplusplus
extern "C" {
#endif

#include <WinCore/Config.h>
#include <WinCore/Types.h>

    typedef struct tagCREATESTRUCTA
    {
        LPVOID    lpCreateParams; /* Pointer to value passed as the last param to CreateWindowEx */
        HINSTANCE hInstance;      /* Handle to the module that owns the window */
        HMENU     hMenu;          /* Handle to the menu to be used by the window */
        HWND      hwndParent;     /* Handle to the parent window */
        int       cy;             /* Height of the window, in pixels */
        int       cx;             /* Width of the window, in pixels */
        int       y;              /* Y-coordinate of the upper-left corner of the window */
        int       x;              /* X-coordinate of the upper-left corner of the window */
        LONG      style;          /* Style flags for the window */
        LPCSTR    lpszName;       /* Name of the window (title string) */
        LPCSTR    lpszClass;      /* Pointer to a null-terminated string specifying the class name */
        DWORD     dwExStyle;      /* Extended style flags for the window */
    } CREATESTRUCTA, * LPCREATESTRUCTA;


#define PM_NOREMOVE 0x0000
#define PM_REMOVE   0x0001

#define CS_VREDRAW  0x0001
#define CS_HREDRAW  0x0002

#define CW_USEDEFAULT ((int)0x80000000)

#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_SHOW             5
#define SW_SHOWDEFAULT      10

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

typedef struct tagRECT 
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, * PRECT, * LPRECT;

#ifdef UNICODE
#else
    typedef WNDCLASSA WNDCLASS;
#endif

WINCORE_API HMODULE GetModuleHandleA(LPCSTR lpModuleName);

WINCORE_API HBRUSH GetSysColorBrush(int nIndex);

WINCORE_API ATOM RegisterClassA(const WNDCLASSA* lpWndClass);

WINCORE_API HWND CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

WINCORE_API BOOL DestroyWindow(HWND hWnd);

WINCORE_API BOOL GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);

WINCORE_API BOOL PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

WINCORE_API void PostQuitMessage(int nExitCode);

WINCORE_API LRESULT DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WINCORE_API LRESULT DispatchMessageA(const MSG* lpMsg);

WINCORE_API BOOL TranslateMessage(const MSG* lpMsg);

WINCORE_API BOOL GetClientRect(HWND hWnd, LPRECT lpRect);

WINCORE_API BOOL ShowWindow(HWND hWnd, int nCmdShow);

WINCORE_API BOOL UpdateWindow(HWND hWnd);

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
