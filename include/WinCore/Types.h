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

#ifndef WinCore_Types_h
#define WinCore_Types_h

#include <stdlib.h>

#define FAR
#define NEAR
#define CALLBACK

typedef void* HANDLE;
typedef HANDLE HWND;
typedef HANDLE HINSTANCE;
typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;
typedef HANDLE HMODULE;
typedef HANDLE HMENU;

typedef void* LPVOID;

typedef unsigned int   UINT;
typedef unsigned int   UINT_PTR;
typedef long           LONG_PTR;
typedef unsigned long  DWORD;
typedef UINT_PTR       WPARAM;
typedef LONG_PTR       LPARAM;
typedef LONG_PTR       LRESULT;
typedef long           LONG;
typedef int            BOOL;
typedef long           ATOM;

typedef char CHAR;
typedef const CHAR* LPCSTR, *PCSTR;

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

#endif
