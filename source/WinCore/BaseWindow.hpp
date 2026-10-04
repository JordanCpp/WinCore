// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_BaseWindow_hpp
#define WinCore_BaseWindow_hpp

#include <string>
#include <WinCore/Windows.h>

class BaseWindow
{
public:
    DWORD        dwExStyle;
    std::string  lpClassName;
    std::string  lpWindowName;
    DWORD        dwStyle;
    int          X;
    int          Y;
    int          nWidth;
    int          nHeight;
    HWND         hWndParent;
    HMENU        hMenu;
    HINSTANCE    hInstance;
    LPVOID       lpParam;
};

#endif
