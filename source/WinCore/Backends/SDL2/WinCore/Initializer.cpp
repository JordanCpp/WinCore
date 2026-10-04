// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL.h>
#include <WinCore/Initializer.hpp>

Initializer::Initializer()
{
	SDL_Init(SDL_INIT_EVERYTHING);
}

Initializer::~Initializer()
{
	SDL_Quit();
}
