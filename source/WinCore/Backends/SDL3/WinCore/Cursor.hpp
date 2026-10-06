// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_Cursor_hpp
#define WinCore_SDL3_Cursor_hpp

#include <WinCore/BaseWindow.hpp>

class Cursor
{
public:
	BOOL GetCursorPos(LPPOINT lpPoint);
	BOOL SetCursorPos(int x, int y);
	int ShowCursor(BOOL bShow);
private:
};

#endif
