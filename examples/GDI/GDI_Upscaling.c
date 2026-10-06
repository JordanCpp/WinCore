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

#define VIRTUAL_WIDTH   320
#define VIRTUAL_HEIGHT  240

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

static DWORD* g_pixelBuffer = NULL;
static BITMAPINFO g_bmi;
static HDC        g_hDC = NULL;
static double     g_timeCounter = 0.0;

static void RenderFrame(DWORD* buffer, int width, int height, double t)
{
    int x, y;

    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            float cx = (float)x - (float)width * 0.5f;
            float cy = (float)y - (float)height * 0.5f;

            float r = sqrtf(cx * cx + cy * cy);
            float angle = atan2f(cy, cx);

            if (r > 1.0f)
            {
                float u = angle * (3.0f / 3.14159265f);
                float v = (100.0f / r) + (float)t;

                int checkX = (int)(u * 4.0f) & 1;
                int checkY = (int)(v * 4.0f) & 1;

                if (checkX ^ checkY)
                {
                    int intensity = (int)(r * 1.5f);
                    if (intensity > 255) intensity = 255;

                    buffer[y * width + x] =
                        ((DWORD)intensity << 16) |
                        ((DWORD)intensity << 8) |
                        (DWORD)intensity;
                }
                else
                {
                    int b = (int)(r * 2.0f);
                    if (b > 255) b = 255;

                    buffer[y * width + x] = (DWORD)b;
                }
            }
            else
            {
                buffer[y * width + x] = 0;
            }
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
            RECT rc;
            GetClientRect(hwnd, &rc);

            int dstW = rc.right - rc.left;
            int dstH = rc.bottom - rc.top;

            if (dstW > 0 && dstH > 0)
            {
                StretchDIBits(
                    paintDC,
                    0, 0, dstW, dstH,
                    0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
                    g_pixelBuffer,
                    &g_bmi,
                    DIB_RGB_COLORS,
                    SRCCOPY
                );
            }
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
    wc.lpszClassName = "WinCoreGDIStretchClass";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);

    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "RegisterClass failed\n");
        return 1;
    }

    hwnd = CreateWindow(
        wc.lpszClassName,
        "Pure C Hardware Upscaling (StretchDIBits)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    if (!hwnd)
    {
        fprintf(stderr, "CreateWindow failed\n");
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    bufferSize = (size_t)VIRTUAL_WIDTH * (size_t)VIRTUAL_HEIGHT * sizeof(DWORD);
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
    g_bmi.bmiHeader.biWidth = VIRTUAL_WIDTH;
    g_bmi.bmiHeader.biHeight = -VIRTUAL_HEIGHT;
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

        if (!running) break;

        g_timeCounter += 0.03;

        RenderFrame(g_pixelBuffer, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, g_timeCounter);

        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }

    free(g_pixelBuffer);
    g_pixelBuffer = NULL;

    return 0;
}
