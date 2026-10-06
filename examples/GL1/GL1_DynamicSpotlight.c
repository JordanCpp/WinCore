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
#include <math.h>
#include <WinCore/Windows.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define GRID_SIZE 40
#define GRID_VERTICES ((GRID_SIZE + 1) * (GRID_SIZE + 1))
#define GRID_INDICES (GRID_SIZE * GRID_SIZE * 4)

float floor_vertices[GRID_VERTICES * 3];
float floor_normals[GRID_VERTICES * 3];
float floor_colors[GRID_VERTICES * 3];
GLushort floor_indices[GRID_INDICES];

void GenerateFloor()
{
    int v_idx = 0;
    int i_idx = 0;
    int i, j;
    float step = 2.0f / (float)GRID_SIZE;

    for (i = 0; i <= GRID_SIZE; ++i)
    {
        float z = -1.0f + (float)i * step;
        for (j = 0; j <= GRID_SIZE; ++j)
        {
            float x = -1.0f + (float)j * step;

            floor_vertices[v_idx] = x * 2.5f;
            floor_normals[v_idx++] = 0.0f;

            floor_vertices[v_idx] = -0.5f;
            floor_normals[v_idx++] = 1.0f;

            floor_vertices[v_idx] = z * 2.5f;
            floor_normals[v_idx++] = 0.0f;

            int color_idx = (v_idx / 3) - 1;
            int r = (i / 4) % 2;
            int c = (j / 4) % 2;
            if (r == c)
            {
                floor_colors[color_idx * 3] = 0.3f;
                floor_colors[color_idx * 3 + 1] = 0.3f;
                floor_colors[color_idx * 3 + 2] = 0.3f;
            }
            else
            {
                floor_colors[color_idx * 3] = 0.8f;
                floor_colors[color_idx * 3 + 1] = 0.8f;
                floor_colors[color_idx * 3 + 2] = 0.8f;
            }
        }
    }

    for (i = 0; i < GRID_SIZE; ++i)
    {
        for (j = 0; j < GRID_SIZE; ++j)
        {
            int p0 = i * (GRID_SIZE + 1) + j;
            int p1 = p0 + 1;
            int p2 = (i + 1) * (GRID_SIZE + 1) + j;
            int p3 = p2 + 1;

            floor_indices[i_idx++] = (GLushort)p0;
            floor_indices[i_idx++] = (GLushort)p1;
            floor_indices[i_idx++] = (GLushort)p3;
            floor_indices[i_idx++] = (GLushort)p2;
        }
    }
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
    }
    break;
    case WM_KEYDOWN:
        if (wParam == 'X' || wParam == 'x')
        {
            SaveScreenshotBMP("OpenGL 1.2 - Dynamic Spotlight.bmp", 800, 600);
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
    HWND     hwnd;
    HDC      hDC;
    HGLRC    hRC;
    int      pixelFormat;
    float    light_angle = 0.0f;

    PIXELFORMATDESCRIPTOR pfd;

    float light_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    float light_diffuse[] = { 0.0f, 0.8f, 1.0f, 1.0f };
    float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    float mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mat_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    float mat_shininess[] = { 30.0f };

    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreDemoClass";
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);

    hwnd = CreateWindow(wc.lpszClassName, "OpenGL 1.2 - Dynamic Spotlight",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);

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

    GenerateFloor();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 15.0f);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, floor_vertices);
    glNormalPointer(GL_FLOAT, 0, floor_normals);
    glColorPointer(3, GL_FLOAT, 0, floor_colors);

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

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-0.666f, 0.666f, -0.5f, 0.5f, 1.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -3.5f);
        glRotatef(25.0f, 1.0f, 0.0f, 0.0f);

        float lx = (float)cos(light_angle) * 1.2f;
        float lz = (float)sin(light_angle) * 1.2f;
        float light_position[] = { lx, 1.5f, lz, 1.0f };
        float spotlight_direction[] = { -lx * 0.5f, -1.5f, -lz * 0.5f };

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotlight_direction);

        glDrawElements(GL_QUADS, GRID_INDICES, GL_UNSIGNED_SHORT, floor_indices);

        light_angle += 0.02f;

        SwapBuffers(hDC);

        Sleep(16);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    return 0;
}
