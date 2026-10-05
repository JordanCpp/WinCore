// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_Ticks_hpp
#define WinCore_SDL3_Ticks_hpp

#include <WinCore/Types.h>

class Ticks
{
public:
	DWORD GetTickCount();
	void Sleep(DWORD dwMilliseconds);
private:
};

#endif
