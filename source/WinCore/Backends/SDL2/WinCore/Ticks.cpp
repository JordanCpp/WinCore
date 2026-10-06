// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL_timer.h>
#include <WinCore/Ticks.hpp>

DWORD Ticks::GetTickCount()
{
	return static_cast<DWORD>(SDL_GetTicks());
}

void Ticks::Sleep(DWORD dwMilliseconds)
{
	SDL_Delay(static_cast<Uint32>(dwMilliseconds));
}