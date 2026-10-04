// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Types_h
#define WinCore_Types_h

#include <stdlib.h>

#define FAR
#define NEAR
#define CALLBACK

#if defined(_WIN32) || defined(__i386__)
    #define WINAPI __stdcall
#else
    #define WINAPI
#endif

typedef void* LPVOID;
typedef void* HANDLE;
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
typedef unsigned long   DWORD;
typedef int             BOOL;
typedef long            LONG;
typedef unsigned short  ATOM;

#if defined(_WIN64) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__) || defined(__LP64__)
    typedef unsigned long long UINT_PTR;
    typedef long long          LONG_PTR;
#else
    typedef unsigned int       UINT_PTR;
    typedef long               LONG_PTR;
#endif

typedef UINT_PTR        WPARAM;
typedef LONG_PTR        LPARAM;
typedef LONG_PTR        LRESULT;

typedef char  CHAR;
typedef const CHAR* LPCSTR, * PCSTR;

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

#endif // WinCore_Types_h
