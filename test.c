// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#define OPENGL_IMPLEMENTATION
#include "OpenGL.h"

#include <stdio.h>
#include <string.h>
#include <WinCore/Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        printf("WM_CREATE\n");
        break;

    case WM_PAINT:
        printf("WM_PAINT\n");
        break;

    case WM_CLOSE:
        printf("WM_CLOSE\n");
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        printf("WM_DESTROY\n");
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main()
{
    WNDCLASS wc;
    MSG      msg;

    memset(&wc, 0, sizeof(WNDCLASS));

    wc.lpszClassName = "WinCoreDemoClass";
    wc.lpszMenuName  = "MainMenu";
    wc.lpfnWndProc   = WndProc;

    RegisterClass(&wc);

    DWORD style = 0;
    HWND  hwnd = CreateWindow(wc.lpszClassName, "Title window", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);

    HDC hDC = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;
    pfd.cDepthBits   = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType   = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);

    HGLRC hRC = wglCreateContext(hDC);

    wglMakeCurrent(hDC, hRC);

    OpenGL_Compatibility_Init(1, 2);

    glViewport(0, 0, 800, 600);

    msg.message = WM_NULL;

    while (msg.message != WM_QUIT)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            break;
        }

        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SwapBuffers(hDC);
    }

    return 0;
}
