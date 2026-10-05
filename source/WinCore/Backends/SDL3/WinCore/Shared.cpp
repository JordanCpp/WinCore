// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Shared.hpp>

void* Shared::Load(const char* path)
{
	_object = SDL_LoadObject(path);

	return _object;
}

void Shared::Unload()
{
	if (_object)
	{
		SDL_UnloadObject(_object);
	}
}

void* Shared::GetFunction(const char* name)
{
	void* func = (void*)SDL_LoadFunction(_object, name);
	
	return func;
}
