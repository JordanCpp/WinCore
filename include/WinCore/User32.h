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

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L

#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L

#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define WS_OVERLAPPEDWINDOW WS_TILEDWINDOW
#define WS_POPUPWINDOW      (WS_POPUP | WS_BORDER | WS_SYSMENU)
#define WS_CHILDWINDOW      WS_CHILD

#define WS_EX_DLGMODALFRAME     0x00000001L
#define WS_EX_NOPARENTNOTIFY    0x00000004L
#define WS_EX_TOPMOST           0x00000008L
#define WS_EX_ACCEPTFILES       0x00000010L
#define WS_EX_TRANSPARENT       0x00000020L
#define WS_EX_MDICHILD          0x00000040L
#define WS_EX_TOOLWINDOW        0x00000080L
#define WS_EX_WINDOWEDGE        0x00000100L
#define WS_EX_CLIENTEDGE        0x00000200L
#define WS_EX_CONTEXTHELP       0x00000400L

#define WS_EX_RIGHT             0x00001000L
#define WS_EX_LEFT              0x00000000L
#define WS_EX_RTLREADING        0x00002000L
#define WS_EX_LTRREADING        0x00000000L
#define WS_EX_LEFTSCROLLBAR     0x00004000L
#define WS_EX_RIGHTSCROLLBAR    0x00000000L

#define WS_EX_CONTROLPARENT     0x00010000L
#define WS_EX_STATICEDGE        0x00020000L
#define WS_EX_APPWINDOW         0x00040000L

#define WS_EX_OVERLAPPEDWINDOW  (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
#define WS_EX_PALETTEWINDOW     (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)

    typedef struct tagPOINT 
    {
        LONG x;
        LONG y;
    } POINT, * PPOINT, * LPPOINT;

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

WINCORE_API BOOL GetWindowRect(HWND hWnd, LPRECT lpRect);

WINCORE_API BOOL GetCursorPos(LPPOINT lpPoint);

WINCORE_API BOOL SetCursorPos(int x, int y);

WINCORE_API int ShowCursor(BOOL bShow);

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
