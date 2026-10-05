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
    wc.lpszClassName = "WinCoreGDIRendererClass";
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);

    hwnd = CreateWindow(
        wc.lpszClassName,
        "WinCore Demo - Pure C Software Rendering (SetDIBitsToDevice)",
        0,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    if (!hwnd)
    {
        return 1;
    }

    hDC = GetDC(hwnd);

    bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(DWORD);
    pixelBuffer = (DWORD*)malloc(bufferSize);
    if (!pixelBuffer)
    {
        return 1;
    }

    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = SCREEN_WIDTH;
    bmi.bmiHeader.biHeight = SCREEN_HEIGHT;
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

        timeCounter += 0.05f;
        {
            int y, x;
            for (y = 0; y < SCREEN_HEIGHT; ++y)
            {
                for (x = 0; x < SCREEN_WIDTH; ++x)
                {
                    float cx = (float)x - (float)SCREEN_WIDTH / 2.0f;
                    float cy = (float)y - (float)SCREEN_HEIGHT / 2.0f;
                    float dist = sqrtf(cx * cx + cy * cy);

                    float wave1 = sinf((float)x * 0.02f + timeCounter);
                    float wave2 = sinf((float)y * 0.03f - timeCounter * 1.5f);
                    float wave3 = sinf(dist * 0.05f - timeCounter);

                    float finalFactor = (wave1 + wave2 + wave3) / 3.0f;

                    int r = (int)((finalFactor * 0.5f + 0.5f) * 255.0f);
                    int g = (int)((cosf(timeCounter) * 0.5f + 0.5f) * 255.0f);
                    int b = (int)((1.0f - (finalFactor * 0.5f + 0.5f)) * 255.0f);

                    DWORD color = ((DWORD)r << 16) | ((DWORD)g << 8) | (DWORD)b;

                    pixelBuffer[y * SCREEN_WIDTH + x] = color;
                }
            }
        }

        SetDIBitsToDevice(
            hDC,
            0, 0,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            0, 0,
            0,
            SCREEN_HEIGHT,
            pixelBuffer,
            &bmi,
            DIB_RGB_COLORS
        );
    }

    free(pixelBuffer);
    DestroyWindow(hwnd);

    return 0;
}
