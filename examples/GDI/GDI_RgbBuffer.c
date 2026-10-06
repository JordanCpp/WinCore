/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <WinCore/Windows.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

static DWORD*      g_pixelBuffer = NULL;
static BITMAPINFO  g_bmi;
static HDC         g_hDC = NULL;
static float       g_timeCounter = 0.0f;

static void RenderFrame(DWORD* buffer, int width, int height, float t)
{
    int x, y;

    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            float cx = (float)x - (float)width * 0.5f;
            float cy = (float)y - (float)height * 0.5f;
            float dist = sqrtf(cx * cx + cy * cy);

            float wave1 = sinf((float)x * 0.02f + t);
            float wave2 = sinf((float)y * 0.03f - t * 1.5f);
            float wave3 = sinf(dist * 0.05f - t);

            float finalFactor = (wave1 + wave2 + wave3) / 3.0f;

            int r = (int)((finalFactor * 0.5f + 0.5f) * 255.0f);
            int g = (int)((cosf(t) * 0.5f + 0.5f) * 255.0f);
            int b = (int)((1.0f - (finalFactor * 0.5f + 0.5f)) * 255.0f);

            DWORD color = ((DWORD)r << 16) | ((DWORD)g << 8) | (DWORD)b;

            buffer[y * width + x] = color;
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        g_hDC = GetDC(hwnd);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        if (g_hDC)
        {
            ReleaseDC(hwnd, g_hDC);
            g_hDC = NULL;
        }

        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC paintDC = BeginPaint(hwnd, &ps);

        if (paintDC && g_pixelBuffer)
        {
            SetDIBitsToDevice(
                paintDC,
                0, 0,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                0, 0,
                0,
                SCREEN_HEIGHT,
                g_pixelBuffer,
                &g_bmi,
                DIB_RGB_COLORS
            );
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main(void)
{
    WNDCLASS wc;
    MSG      msg;
    HWND     hwnd;
    size_t   bufferSize;
    BOOL     running;

    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreGDIRendererClass";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = NULL;

    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "RegisterClass failed\n");
        return 1;
    }

    hwnd = CreateWindow(
        wc.lpszClassName,
        "Pure C Software Rendering (SetDIBitsToDevice)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    if (!hwnd)
    {
        fprintf(stderr, "CreateWindow failed\n");
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    bufferSize = (size_t)SCREEN_WIDTH * (size_t)SCREEN_HEIGHT * sizeof(DWORD);
    g_pixelBuffer = (DWORD*)malloc(bufferSize);
    if (!g_pixelBuffer)
    {
        fprintf(stderr, "Out of memory\n");
        DestroyWindow(hwnd);
        return 1;
    }

    memset(g_pixelBuffer, 0, bufferSize);

    memset(&g_bmi, 0, sizeof(BITMAPINFO));
    g_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    g_bmi.bmiHeader.biWidth = SCREEN_WIDTH;
    g_bmi.bmiHeader.biHeight = -SCREEN_HEIGHT;
    g_bmi.bmiHeader.biPlanes = 1;
    g_bmi.bmiHeader.biBitCount = 32;
    g_bmi.bmiHeader.biCompression = BI_RGB;
    g_bmi.bmiHeader.biSizeImage = (DWORD)bufferSize;

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

        g_timeCounter += 0.05f;
        RenderFrame(g_pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, g_timeCounter);

        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }

    free(g_pixelBuffer);
    g_pixelBuffer = NULL;

    return 0;
}
