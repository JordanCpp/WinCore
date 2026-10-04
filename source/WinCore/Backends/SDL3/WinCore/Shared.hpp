// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_Shared_hpp
#define WinCore_SDL3_Shared_hpp

#include <SDL3/SDL_loadso.h>

class Shared
{
public:
	void* Load(const char* path);
	void Unload();
	void* GetFunction(const char* path);
private:
	SDL_SharedObject* _object;
};

#endif