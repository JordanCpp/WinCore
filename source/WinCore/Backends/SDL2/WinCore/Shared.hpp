// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL2_Shared_hpp
#define WinCore_SDL2_Shared_hpp

#include <SDL.h>

class Shared
{
public:
	void* Load(const char* path);
	void Unload();
	void* GetFunction(const char* path);
private:
	void* _object;
};

#endif