// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/BaseWindow.hpp>

BaseWindow::BaseWindow() :
    dwExStyle(0),
    dwStyle(0),
    X(0),
    Y(0),
    nWidth(0),
    nHeight(0),
    hWndParent(NULL),
    hMenu(NULL),
    hInstance(NULL),
    lpParam(NULL),
    _paintValid(TRUE)
{

}

void BaseWindow::SetPaintValid(BOOL valid)
{
	_paintValid = valid;
}

BOOL BaseWindow::IsPaintValid() const
{
	return _paintValid;
}