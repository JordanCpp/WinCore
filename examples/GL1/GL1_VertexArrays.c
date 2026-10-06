/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#define OPENGL_IMPLEMENTATION
#include "OpenGL.h"

#include <stdio.h>
#include <string.h>
#include <WinCore/Windows.h>

float vertices[] = 
{
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f
};

float colors[] = 
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f
};

GLubyte indices[] =
{
    0, 1, 2, 3,
    4, 5, 6, 7,
    3, 2, 6, 7,
    0, 1, 5, 4,
    1, 5, 6, 2,
    0, 4, 7, 3
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
    {
        int width = (int)LOWORD(lParam);
        int height = (int)HIWORD(lParam);

        if (height == 0)
        {
            height = 1;
        }

        glViewport(0, 0, width, height);
    }
    break;
    case WM_KEYDOWN:
        if (wParam == 'X' || wParam == 'x')
        {
            SaveScreenshotBMP("OpenGL 1.2 - Vertex Arrays.bmp", 800, 600);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
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
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "OpenGL 1.2 - Vertex Arrays",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);

    HDC hDC = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);

    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    OpenGL_Compatibility_Init(1, 2);
    glViewport(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    msg.message = WM_NULL;
    float angle = 0.0f;

    while (msg.message != WM_QUIT)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT) break;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float fov = 0.5f;
        float aspect = 800.0f / 600.0f;
        glFrustum(-fov * aspect, fov * aspect, -fov, fov, 1.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.0f);
        glRotatef(angle, 1.0f, 1.0f, 0.5f);

        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

        angle += 0.5f;

        SwapBuffers(hDC);

        Sleep(16);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    return 0;
}
