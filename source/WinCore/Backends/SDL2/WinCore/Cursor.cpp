// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL_mouse.h>
#include <WinCore/Cursor.hpp>

static const int SDL_ENABLE = 1;
static const int SDL_DISABLE = 0;

BOOL Cursor::GetCursorPos(LPPOINT lpPoint)
{
    if (!lpPoint)
    {
        return FALSE;
    }

    int globalX = 0;
    int globalY = 0;
    SDL_GetGlobalMouseState(&globalX, &globalY);

    lpPoint->x = globalX;
    lpPoint->y = globalY;

    return TRUE;
}

BOOL Cursor::SetCursorPos(int x, int y)
{
    return SDL_WarpMouseGlobal(x, y) == 0;
}

int Cursor::ShowCursor(BOOL bShow)
{
    static int cursorCounter = 0;

    if (bShow)
    {
        if (++cursorCounter == 1)
        {
            SDL_ShowCursor(SDL_ENABLE);
        }
    }
    else
    {
        if (--cursorCounter == 0)
        {
            SDL_ShowCursor(SDL_DISABLE);
        }
    }

    return cursorCounter;
}
