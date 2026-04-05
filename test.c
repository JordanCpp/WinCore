/*
Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp)

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, see <https://gnu.org>.
*/

#include <stdio.h>
#include <WinCore/GL/GL.h>
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

    case WM_DESTROY:
        printf("WM_DESTROY\n");
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main(int argc, char* argv[])
{
    WNDCLASS wc;
    MSG      msg;

    wc.lpszClassName = "asdad";
    wc.lpszMenuName  = "asdad";
    wc.lpfnWndProc   = WndProc;

    RegisterClass(&wc);

    DWORD style = 0;
    HWND  hwnd  = CreateWindow(wc.lpszClassName, "Title window", style, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL);

    HDC hDC = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;

    HGLRC hRC = wglCreateContext(hDC);

    wglMakeCurrent(hDC, hRC);

    glViewport(0, 0, 800, 600);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);

        glClearColor(255, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SwapBuffers(hDC);
    }

    return 0;
}
