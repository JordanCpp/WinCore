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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main(void)
{
    WNDCLASS wc;
    MSG      msg;
    HWND     hwnd;
    HDC      hDC;
    size_t   bufferSize;
    DWORD* pixelBuffer;
    BITMAPINFO bmi;
    float    timeCounter;

    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreGDIStretchClass";
    wc.lpfnWndProc   = WndProc;
    RegisterClass(&wc);

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
        return 1;
    }

    hDC = GetDC(hwnd);

    bufferSize = VIRTUAL_WIDTH * VIRTUAL_HEIGHT * sizeof(DWORD);
    pixelBuffer = (DWORD*)malloc(bufferSize);
    if (!pixelBuffer)
    {
        return 1;
    }

    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = VIRTUAL_WIDTH;
    bmi.bmiHeader.biHeight = VIRTUAL_HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = (DWORD)bufferSize;

    msg.message = WM_NULL;
    timeCounter = 0.0f;

    while (msg.message != WM_QUIT)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT) break;

        timeCounter += 0.03f;
        {
            int y, x;
            for (y = 0; y < VIRTUAL_HEIGHT; ++y)
            {
                for (x = 0; x < VIRTUAL_WIDTH; ++x)
                {
                    float cx = (float)x - (float)VIRTUAL_WIDTH / 2.0f;
                    float cy = (float)y - (float)VIRTUAL_HEIGHT / 2.0f;

                    float r = sqrtf(cx * cx + cy * cy);
                    float angle = atan2f(cy, cx);

                    if (r > 1.0f)
                    {
                        float u = angle * (3.0f / 3.14159265f);
                        float v = (100.0f / r) + timeCounter;

                        int checkX = (int)(u * 4.0f) & 1;
                        int checkY = (int)(v * 4.0f) & 1;

                        if (checkX ^ checkY)
                        {
                            int intensity = (int)(r * 1.5f);
                            if (intensity > 255) intensity = 255;
                            pixelBuffer[y * VIRTUAL_WIDTH + x] = ((DWORD)intensity << 16) | ((DWORD)intensity << 8) | (DWORD)intensity;
                        }
                        else
                        {
                            int b = (int)(r * 2.0f);
                            if (b > 255) b = 255;
                            pixelBuffer[y * VIRTUAL_WIDTH + x] = (DWORD)b;
                        }
                    }
                    else
                    {
                        pixelBuffer[y * VIRTUAL_WIDTH + x] = 0;
                    }
                }
            }
        }

        StretchDIBits(
            hDC,
            0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
            0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
            pixelBuffer,
            &bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }

    free(pixelBuffer);
    ReleaseDC(hwnd, hDC);
    DestroyWindow(hwnd);

    return 0;
}
