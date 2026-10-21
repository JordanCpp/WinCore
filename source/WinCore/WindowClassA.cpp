// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/WindowClassA.hpp>

WindowClassA::WindowClassA() :
    style(0),
    lpfnWndProc(NULL),
    cbClsExtra(0),
    cbWndExtra(0),
    hInstance(NULL),
    hIcon(NULL),
    hCursor(NULL),
    hbrBackground(NULL),
    lpszMenuName(),
    lpszClassName(),
    atom(0)
{
}
