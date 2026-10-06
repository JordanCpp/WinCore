// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_AsyncKey_hpp
#define WinCore_SDL3_AsyncKey_hpp

#include <SDL3/SDL_scancode.h>
#include <WinCore/BaseWindow.hpp>

class AsyncKey
{
public:
	SDL_Scancode TranslateVirtualKeyToScancode(int vKey);
	SHORT GetAsyncKeyStateImpl(int vKey);
private:
};

#endif
