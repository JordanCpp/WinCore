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

#define SPHERE_SLICES 20
#define SPHERE_STACKS 20
#define TOTAL_VERTICES ((SPHERE_SLICES + 1) * (SPHERE_STACKS + 1))
#define TOTAL_INDICES (SPHERE_SLICES * SPHERE_STACKS * 4)

float sphere_vertices[TOTAL_VERTICES * 3];
float sphere_normals[TOTAL_VERTICES * 3];
GLushort sphere_indices[TOTAL_INDICES];

void GenerateSphere()
{
    int v_idx = 0;
    int i_idx = 0;
    int i, j;

    for (i = 0; i <= SPHERE_STACKS; ++i)
    {
        float lat = (float)M_PI * (-0.5f + (float)i / SPHERE_STACKS);
        float sin_lat = (float)sin(lat);
        float cos_lat = (float)cos(lat);

        for (j = 0; j <= SPHERE_SLICES; ++j)
        {
            float lon = 2.0f * (float)M_PI * (float)j / SPHERE_SLICES;
            float sin_lon = (float)sin(lon);
            float cos_lon = (float)cos(lon);

            float x = cos_lon * cos_lat;
            float y = sin_lon * cos_lat;
            float z = sin_lat;

            sphere_normals[v_idx] = x;
            sphere_vertices[v_idx++] = x * 0.6f;

            sphere_normals[v_idx] = y;
            sphere_vertices[v_idx++] = y * 0.6f;

            sphere_normals[v_idx] = z;
            sphere_vertices[v_idx++] = z * 0.6f;
        }
    }

    for (i = 0; i < SPHERE_STACKS; ++i)
    {
        for (j = 0; j < SPHERE_SLICES; ++j)
        {
            int p0 = i * (SPHERE_SLICES + 1) + j;
            int p1 = p0 + 1;
            int p2 = (i + 1) * (SPHERE_SLICES + 1) + j;
            int p3 = p2 + 1;

            sphere_indices[i_idx++] = (GLushort)p0;
            sphere_indices[i_idx++] = (GLushort)p1;
            sphere_indices[i_idx++] = (GLushort)p3;
            sphere_indices[i_idx++] = (GLushort)p2;
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int use_smooth = 1;

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
                SaveScreenshotBMP("OpenGL 1.2 - Gouraud vs Flat.bmp", w, h);
            }
        }

        if (wParam == VK_SPACE)
        {
            use_smooth = !use_smooth;
            if (use_smooth)
            {
                glShadeModel(GL_SMOOTH);
            }
            else
            {
                glShadeModel(GL_FLAT);
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

    float light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float light_diffuse[] = { 0.8f, 0.5f, 0.2f, 1.0f };
    float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light_position[] = { 2.0f, 2.0f, 2.0f, 1.0f };

    float mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mat_shininess[] = { 50.0f };

    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreDemoClass";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = NULL;

    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "RegisterClass failed\n");
        return 1;
    }

    hwnd = CreateWindow(
        wc.lpszClassName,
        "OpenGL 1.2 - Gouraud vs Flat (Press SPACE)",
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

    GenerateSphere();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, sphere_vertices);
    glNormalPointer(GL_FLOAT, 0, sphere_normals);

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

        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-0.666f, 0.666f, -0.5f, 0.5f, 1.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.0f);
        glRotatef(angle, 0.5f, 1.0f, 0.0f);

        glDrawElements(GL_QUADS, TOTAL_INDICES, GL_UNSIGNED_SHORT, sphere_indices);

        angle += 0.3f;

        SwapBuffers(hDC);

        Sleep(16);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);

    return 0;
}
