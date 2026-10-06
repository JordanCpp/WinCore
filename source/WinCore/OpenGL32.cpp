// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/OpenGLFunc.hpp>
#include <WinCore/Application.hpp>

PROC wglGetProcAddress(LPCSTR unnamedParam1)
{
	return LoadGLFunction(unnamedParam1);
}

BOOL wglDeleteContext(HGLRC hglrc)
{
    (void)hglrc;

    return TRUE;
}
