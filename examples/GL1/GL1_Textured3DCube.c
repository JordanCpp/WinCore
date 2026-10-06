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

#define TEX_SIZE 64

GLuint textureID;

void CreateCheckerboardTexture()
{
    int i, j, c;
    GLubyte textureData[TEX_SIZE][TEX_SIZE][4];

    for (i = 0; i < TEX_SIZE; i++)
    {
        for (j = 0; j < TEX_SIZE; j++)
        {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;

            textureData[i][j][0] = (GLubyte)c;
            textureData[i][j][1] = (GLubyte)c;
            textureData[i][j][2] = (GLubyte)255;
            textureData[i][j][3] = (GLubyte)255;
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
}

void DrawCube()
{
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();
}

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
        return 0;
    }

    case WM_KEYDOWN:
        if (wParam == 'X' || wParam == 'x')
        {
            RECT rc;
            GetClientRect(hwnd, &rc);

            int w = rc.right - rc.left;
            int h = rc.bottom - rc.top;

            if (w > 0 && h > 0)
            {
                SaveScreenshotBMP("OpenGL 1.2 - Textured 3D Cube.bmp", w, h);
            }
        }
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main()
{
    WNDCLASS wc;
    MSG      msg;
    HWND     hwnd;
    HDC      hDC;
    HGLRC    hRC;
    int      pixelFormat;
    float    angle = 0.0f;
    BOOL     running;

    PIXELFORMATDESCRIPTOR pfd;

    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreDemoClass";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);

    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "RegisterClass failed\n");
        return 1;
    }

    hwnd = CreateWindow(
        wc.lpszClassName,
        "OpenGL 1.2 - Textured 3D Cube",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, wc.hInstance, NULL);

    if (!hwnd)
    {
        fprintf(stderr, "CreateWindow failed\n");
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    hDC = GetDC(hwnd);
    if (!hDC)
    {
        fprintf(stderr, "GetDC failed\n");
        DestroyWindow(hwnd);
        return 1;
    }

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);

    hRC = wglCreateContext(hDC);
    if (!hRC)
    {
        fprintf(stderr, "wglCreateContext failed\n");
        ReleaseDC(hwnd, hDC);
        DestroyWindow(hwnd);
        return 1;
    }

    wglMakeCurrent(hDC, hRC);

    OpenGL_Compatibility_Init(1, 2);

    {
        RECT rc;
        GetClientRect(hwnd, &rc);
        glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    CreateCheckerboardTexture();

    msg.message = WM_NULL;
    running = TRUE;

    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = FALSE;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!running)
        {
            break;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            RECT rc;
            GetClientRect(hwnd, &rc);

            int   w = rc.right - rc.left;
            int   h = rc.bottom - rc.top;
            float aspect = (h > 0) ? ((float)w / (float)h) : 1.0f;
            float fov = 0.5f;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(-fov * aspect, fov * aspect, -fov, fov, 1.0f, 100.0f);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.0f);
        glRotatef(angle, 1.0f, 1.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, textureID);

        DrawCube();

        angle += 0.5f;

        SwapBuffers(hDC);

        Sleep(16);
    }

    glDeleteTextures(1, &textureID);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);

    return 0;
}
