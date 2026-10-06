// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL3/SDL_keyboard.h>
#include <WinCore/AsyncKey.hpp>

SDL_Scancode AsyncKey::TranslateVirtualKeyToScancode(int vKey)
{
	switch (vKey)
	{

	case 0x1B: return SDL_SCANCODE_ESCAPE;    // VK_ESCAPE
	case 0x20: return SDL_SCANCODE_SPACE;     // VK_SPACE
	case 0x0D: return SDL_SCANCODE_RETURN;    // VK_RETURN
	case 0x09: return SDL_SCANCODE_TAB;       // VK_TAB
	case 0x08: return SDL_SCANCODE_BACKSPACE; // VK_BACK

	case 0x25: return SDL_SCANCODE_LEFT;      // VK_LEFT
	case 0x26: return SDL_SCANCODE_UP;        // VK_UP
	case 0x27: return SDL_SCANCODE_RIGHT;     // VK_RIGHT
	case 0x28: return SDL_SCANCODE_DOWN;      // VK_DOWN

	case 0x10: return SDL_SCANCODE_LSHIFT;    // VK_SHIFT
	case 0x11: return SDL_SCANCODE_LCTRL;     // VK_CONTROL
	case 0x12: return SDL_SCANCODE_LALT;      // VK_MENU (ALT)

	default:
		if (vKey >= 'A' && vKey <= 'Z')
		{
			return static_cast<SDL_Scancode>(SDL_SCANCODE_A + (vKey - 'A'));
		}
		if (vKey >= '0' && vKey <= '9')
		{
			if (vKey == '0') return SDL_SCANCODE_0;
			return static_cast<SDL_Scancode>(SDL_SCANCODE_1 + (vKey - '1'));
		}
		break;
	}

	return SDL_SCANCODE_UNKNOWN;
}

SHORT AsyncKey::GetAsyncKeyStateImpl(int vKey)
{
	SDL_Scancode scancode = TranslateVirtualKeyToScancode(vKey);

	if (scancode == SDL_SCANCODE_UNKNOWN)
	{
		return 0;
	}

	int numKeys = 0;
	const bool* keyboardState = SDL_GetKeyboardState(&numKeys);

	if (keyboardState && scancode < numKeys)
	{
		if (keyboardState[scancode])
		{
			return static_cast<SHORT>(0x8000);
		}
	}

	return 0;
}
