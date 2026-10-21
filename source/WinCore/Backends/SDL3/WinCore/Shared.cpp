// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Shared.hpp>

Shared::Shared() :
    _object(NULL)
{
}

Shared::~Shared()
{
    Unload();
}

bool Shared::Load(const char* path)
{
    if (!path || !*path)
    {
        return false;
    }

    if (_object)
    {
        return true;
    }

    _object = SDL_LoadObject(path);

    return _object != NULL;
}

void Shared::Unload()
{
    if (_object)
    {
        SDL_UnloadObject(_object);
        _object = NULL;
    }
}

void* Shared::GetFunction(const char* name)
{
    if (!_object || !name || !*name)
    {
        return NULL;
    }

    return (void*)SDL_LoadFunction(_object, name);
}

bool Shared::IsLoaded() const
{
    return _object != NULL;
}
