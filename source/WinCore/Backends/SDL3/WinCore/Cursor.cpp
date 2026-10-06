// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL3/SDL_mouse.h>
#include <WinCore/Cursor.hpp>

BOOL Cursor::GetCursorPos(LPPOINT lpPoint)
{
	if (!lpPoint)
	{
		return FALSE;
	}

	float globalX = 0.0f;
	float globalY = 0.0f;

	SDL_GetGlobalMouseState(&globalX, &globalY);

	lpPoint->x = static_cast<LONG>(globalX);
	lpPoint->y = static_cast<LONG>(globalY);

	return TRUE;
}

BOOL Cursor::SetCursorPos(int x, int y)
{
	return SDL_WarpMouseGlobal(static_cast<float>(x), static_cast<float>(y));
}

int Cursor::ShowCursor(BOOL bShow)
{
	static int cursorCounter = 0;

	if (bShow)
	{
		cursorCounter++;
		if (cursorCounter == 0)
		{
			SDL_ShowCursor();
		}
	}
	else
	{
		cursorCounter--;
		if (cursorCounter == -1)
		{
			SDL_HideCursor();
		}
	}

	return cursorCounter;
}
