// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/OpenGLFunc.hpp>
#include <WinCore/Application.hpp>

HGLRC wglCreateContextAttribsARB(HDC hdc, HGLRC hShareContext, const int* attribList)
{
    Window* window = MainApplication()._windowManager.Find((HWND)hdc);

    if (window)
    {
        return window->wglCreateContextAttribsARB(hShareContext, attribList);
    }

    return NULL;
}

PROC wglGetProcAddress(LPCSTR unnamedParam1)
{
    if (!unnamedParam1)
    {
        return NULL;
    }


    if (strcmp(unnamedParam1, "wglCreateContextAttribsARB") == 0)
    {
        return reinterpret_cast<void*>(wglCreateContextAttribsARB);
    }

    return MainApplication()._openGLFuncs.GetFunction(unnamedParam1);
}

BOOL wglDeleteContext(HGLRC hglrc)
{
    (void)hglrc;

    return TRUE;
}
