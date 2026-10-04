// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_WindowClassA_hpp
#define WinCore_WindowClassA_hpp

#include <string>
#include <WinCore/Windows.h>

class WindowClassA
{
public:
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    std::string lpszMenuName;
    std::string lpszClassName;
};

#endif
