// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Types_h
#define WinCore_Types_h

#include <stdlib.h>

#define FAR
#define NEAR
#define CALLBACK
#define WINAPI __stdcall

typedef void*  LPVOID;
typedef void*  HANDLE;
typedef HANDLE HWND;
typedef HANDLE HINSTANCE;
typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;
typedef HANDLE HMODULE;
typedef HANDLE HMENU;
typedef HANDLE HDC;
typedef HANDLE HGLRC;

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int    UINT;
typedef unsigned int    UINT_PTR;
typedef long            LONG_PTR;
typedef unsigned long   DWORD;
typedef UINT_PTR        WPARAM;
typedef LONG_PTR        LPARAM;
typedef LONG_PTR        LRESULT;
typedef long            LONG;
typedef int             BOOL;
typedef long            ATOM;

typedef char  CHAR;
typedef const CHAR* LPCSTR, *PCSTR;

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

#endif
