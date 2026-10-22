// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <WinCore/Initializer.hpp>
#include <WinCore/Application.hpp>

Initializer::Initializer() :
    _initialized(false)
{
    _initialized = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if (!_initialized)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "WinCore: SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());

        MainApplication().SetLastError(ERROR_INVALID_PARAMETER);
    }
}

Initializer::~Initializer()
{
    if (_initialized)
    {
        SDL_Quit();
        _initialized = false;
    }
}

bool Initializer::IsInitialized() const
{
    return _initialized;
}
