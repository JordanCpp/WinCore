/* Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later. */

#ifndef WinCore_Types_h
#define WinCore_Types_h

#include <stddef.h>

#if defined(_WIN32) && defined(_M_IX86)
    #define WINAPI   __stdcall
    #define CALLBACK __stdcall
#elif defined(_WIN32) && defined(__i386__)
    #define WINAPI   __attribute__((stdcall))
    #define CALLBACK __attribute__((stdcall))
#else
    #define WINAPI
    #define CALLBACK
#endif

#define FAR
#define NEAR

typedef int (CALLBACK* PROC)();

typedef void* LPVOID;
typedef const void* LPCVOID;

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

#if defined(_WIN32)
    typedef unsigned long DWORD;
#else
    typedef unsigned int  DWORD;
#endif

typedef int             BOOL;

#if defined(_WIN32)
    typedef long LONG;
#else
    typedef int  LONG;
#endif

typedef unsigned short ATOM;

#if defined(_WIN64) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__) || defined(__LP64__) || defined(__aarch64__)
    typedef unsigned long long UINT_PTR;
    typedef unsigned long long DWORD_PTR;
    typedef long long          LONG_PTR;
#else
    typedef unsigned int       UINT_PTR;
    typedef unsigned long      DWORD_PTR;
    typedef long               LONG_PTR;
#endif

typedef UINT_PTR        WPARAM;
typedef LONG_PTR        LPARAM;
typedef LONG_PTR        LRESULT;

typedef short           SHORT;
typedef unsigned short  USHORT;

typedef char  CHAR;
typedef char* LPSTR;
typedef const CHAR* LPCSTR;
typedef const CHAR* PCSTR;

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w) ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w) ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

#define MAKEWORD(a, b)    ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)    ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define MAKELPARAM(l, h)  ((LPARAM)(DWORD)MAKELONG(l, h))
#define MAKEWPARAM(l, h)  ((WPARAM)(DWORD)MAKELONG(l, h))
#define MAKELRESULT(l, h) ((LRESULT)(DWORD)MAKELONG(l, h))

#ifndef TRUE
    #define TRUE  1
#endif
#ifndef FALSE
    #define FALSE 0
#endif

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define MAX_PATH 260

typedef char wincore_check_dword[sizeof(DWORD) == 4 ? 1 : -1];
typedef char wincore_check_long[sizeof(LONG) == 4 ? 1 : -1];
typedef char wincore_check_uint[sizeof(UINT) == 4 ? 1 : -1];
typedef char wincore_check_word[sizeof(WORD) == 2 ? 1 : -1];

#endif
