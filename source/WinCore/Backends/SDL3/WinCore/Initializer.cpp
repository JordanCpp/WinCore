// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL3/SDL_init.h>
#include <WinCore/Initializer.hpp>

Initializer::Initializer()
{
	SDL_Init(SDL_INIT_VIDEO);
}

Initializer::~Initializer()
{
	SDL_Quit();
}
