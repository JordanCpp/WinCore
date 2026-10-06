// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL_video.h>
#include <WinCore/OpenGLFunc.hpp>

void* LoadGLFunction(const char* name)
{
	return (void*)SDL_GL_GetProcAddress(name);
}
