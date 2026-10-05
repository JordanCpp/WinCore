/* Copyright(C) 2026 Evgeny Zoshchuk(JordanCpp).Licensed under LGPL - 3.0 - or -later. */

#define OPENGL_IMPLEMENTATION
#include "OpenGL.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <WinCore/Windows.h>

#define TEX_WIDTH  64
#define TEX_HEIGHT 64

float quad_vertices[] =
{
    -1.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
     1.0f,  1.0f,  0.0f,
    -1.0f,  1.0f,  0.0f
};

float tex_coords_0[] =
{
    0.0f, 0.0f,
    2.0f, 0.0f,
    2.0f, 2.0f,
    0.0f, 2.0f
};

float tex_coords_1[] =
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

GLubyte quad_indices[] = { 0, 1, 2, 3 };

GLuint tex_id_0;
GLuint tex_id_1;

void GenerateTextures()
{
    GLubyte image_0[TEX_HEIGHT][TEX_WIDTH][3];
    GLubyte image_1[TEX_HEIGHT][TEX_WIDTH][3];
    int i, j;

    for (i = 0; i < TEX_HEIGHT; i++)
    {
        for (j = 0; j < TEX_WIDTH; j++)
        {
            int c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
            image_0[i][j][0] = (GLubyte)(c * 0.5f);
            image_0[i][j][1] = (GLubyte)0;
            image_0[i][j][2] = (GLubyte)0;
        }
    }

    for (i = 0; i < TEX_HEIGHT; i++)
    {
        for (j = 0; j < TEX_WIDTH; j++)
        {
            float dx = (float)(j - TEX_WIDTH / 2);
            float dy = (float)(i - TEX_HEIGHT / 2);
            float dist = (float)sqrt(dx * dx + dy * dy);
            int c = (dist < 24.0f) ? 255 : 0;
            image_1[i][j][0] = (GLubyte)0;
            image_1[i][j][1] = (GLubyte)c;
            image_1[i][j][2] = (GLubyte)c;
        }
    }

    glGenTextures(1, &tex_id_0);
    glBindTexture(GL_TEXTURE_2D, tex_id_0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, image_0);

    glGenTextures(1, &tex_id_1);
    glBindTexture(GL_TEXTURE_2D, tex_id_1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, image_1);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == 'X' || wParam == 'x')
        {
            SaveScreenshotBMP("OpenGL 1.2 - Multitexture Blend.bmp", 800, 600);
        }
        break;
    case WM_CLOSE:   PostQuitMessage(0); break;
    case WM_DESTROY: PostQuitMessage(0); break;
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

    PIXELFORMATDESCRIPTOR pfd;

    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreDemoClass";
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);

    hwnd = CreateWindow(wc.lpszClassName, "OpenGL 1.2 - Multitexture Blend",
        0, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);

    hDC = GetDC(hwnd);

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    OpenGL_Compatibility_Init(1, 2);
    glViewport(0, 0, 800, 600);

    GenerateTextures();

    glEnable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id_0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id_1);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnableClientState(GL_VERTEX_ARRAY);

    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_0);

    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_1);

    glVertexPointer(3, GL_FLOAT, 0, quad_vertices);

    msg.message = WM_NULL;

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
        glFrustum(-0.666f, 0.666f, -0.5f, 0.5f, 1.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.5f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, quad_indices);

        angle += 0.2f;

        SwapBuffers(hDC);
    }

    glClientActiveTexture(GL_TEXTURE1);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    return 0;
}
