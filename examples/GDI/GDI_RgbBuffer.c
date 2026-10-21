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

static DWORD* g_pixelBuffer = NULL;
static BITMAPINFO  g_bmi;
static int         g_windowWidth = SCREEN_WIDTH;
static int         g_windowHeight = SCREEN_HEIGHT;
static float       g_timeCounter = 0.0f;
static unsigned    g_frameCounter = 0;

static void RenderFrame(DWORD* buffer, int width, int height, float t)
{
    int x, y;

    float cosT = cosf(t);

    for (y = 0; y < height; ++y)
    {
        float fy = (float)y;
        float cy = fy - (float)height * 0.5f;

        for (x = 0; x < width; ++x)
        {
            float fx = (float)x;
            float cx = fx - (float)width * 0.5f;
            float dist = sqrtf(cx * cx + cy * cy);

            float wave1 = sinf(fx * 0.02f + t);
            float wave2 = sinf(fy * 0.03f - t * 1.5f);
            float wave3 = sinf(dist * 0.05f - t);

            float finalFactor = (wave1 + wave2 + wave3) / 3.0f;

            int r = (int)((finalFactor * 0.5f + 0.5f) * 255.0f);
            int g = (int)((cosT * 0.5f + 0.5f) * 255.0f);
            int b = (int)((1.0f - (finalFactor * 0.5f + 0.5f)) * 255.0f);

            if (r < 0) r = 0; if (r > 255) r = 255;
            if (g < 0) g = 0; if (g > 255) g = 255;
            if (b < 0) b = 0; if (b > 255) b = 255;

            DWORD color = ((DWORD)r << 16) | ((DWORD)g << 8) | (DWORD)b;

            buffer[y * width + x] = color;
        }
    }
}

static const char* MsgName(UINT msg)
{
    switch (msg)
    {
    case WM_NULL:          return "WM_NULL";
    case WM_CREATE:        return "WM_CREATE";
    case WM_DESTROY:       return "WM_DESTROY";
    case WM_MOVE:          return "WM_MOVE";
    case WM_SIZE:          return "WM_SIZE";
    case WM_ACTIVATE:      return "WM_ACTIVATE";
    case WM_SETFOCUS:      return "WM_SETFOCUS";
    case WM_KILLFOCUS:     return "WM_KILLFOCUS";
    case WM_ENABLE:        return "WM_ENABLE";
    case WM_PAINT:         return "WM_PAINT";
    case WM_CLOSE:         return "WM_CLOSE";
    case WM_QUIT:          return "WM_QUIT";
    case WM_ERASEBKGND:    return "WM_ERASEBKGND";
    case WM_SHOWWINDOW:    return "WM_SHOWWINDOW";
    case WM_NCCREATE:      return "WM_NCCREATE";
    case WM_NCDESTROY:     return "WM_NCDESTROY";
    case WM_KEYDOWN:       return "WM_KEYDOWN";
    case WM_KEYUP:         return "WM_KEYUP";
    case WM_CHAR:          return "WM_CHAR";
    case WM_MOUSEMOVE:     return "WM_MOUSEMOVE";
    case WM_LBUTTONDOWN:   return "WM_LBUTTONDOWN";
    case WM_LBUTTONUP:     return "WM_LBUTTONUP";
    case WM_RBUTTONDOWN:   return "WM_RBUTTONDOWN";
    case WM_RBUTTONUP:     return "WM_RBUTTONUP";
    default:               return "WM_???";
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    printf("[WndProc] hwnd=%p msg=0x%04X (%s) wParam=%llu lParam=%lld\n",
        (void*)hwnd,
        (unsigned)msg,
        MsgName(msg),
        (unsigned long long)wParam,
        (long long)lParam);

    switch (msg)
    {
    case WM_CREATE:
        printf("           -> WM_CREATE handled\n");
        return 0;

    case WM_SIZE:
    {
        int w = (int)LOWORD(lParam);
        int h = (int)HIWORD(lParam);
        if (w <= 0) w = 1;
        if (h <= 0) h = 1;
        g_windowWidth = w;
        g_windowHeight = h;
        printf("           -> WM_SIZE %d x %d\n", w, h);
        return 0;
    }

    case WM_KEYDOWN:
        printf("           -> WM_KEYDOWN vk=0x%04X\n", (unsigned)wParam);
        if (wParam == VK_ESCAPE)
        {
            printf("           -> VK_ESCAPE -> DestroyWindow\n");
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_CLOSE:
        printf("           -> WM_CLOSE -> DestroyWindow\n");
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        printf("           -> WM_DESTROY -> PostQuitMessage(0)\n");
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        printf("           -> WM_PAINT begin\n");
        PAINTSTRUCT ps;
        HDC paintDC = BeginPaint(hwnd, &ps);

        if (!paintDC)
        {
            fprintf(stderr, "           -> [WM_PAINT] BeginPaint FAILED\n");
            EndPaint(hwnd, &ps);
            return 0;
        }

        printf("           -> BeginPaint returned DC=%p\n", (void*)paintDC);

        if (!g_pixelBuffer)
        {
            fprintf(stderr, "           -> [WM_PAINT] g_pixelBuffer == NULL\n");
            EndPaint(hwnd, &ps);
            return 0;
        }

        printf("           -> StretchDIBits dest=(0,0,%d,%d) src=(0,0,%d,%d)\n",
            g_windowWidth, g_windowHeight, SCREEN_WIDTH, SCREEN_HEIGHT);

        int copied = StretchDIBits(
            paintDC,
            0, 0, g_windowWidth, g_windowHeight,
            0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
            g_pixelBuffer,
            &g_bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        if (copied == 0)
        {
            fprintf(stderr, "           -> [WM_PAINT] StretchDIBits FAILED\n");
        }
        else
        {
            printf("           -> StretchDIBits OK (returned %d)\n", copied);
        }

        EndPaint(hwnd, &ps);
        printf("           -> WM_PAINT end\n");
        return 0;
    }

    case WM_ERASEBKGND:
        printf("           -> WM_ERASEBKGND suppressed\n");
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

    printf("[STEP] RegisterClass\n");
    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreGDIRendererClass";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);

    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "[FAIL] RegisterClass\n");
        return 1;
    }

    printf("[STEP] CreateWindow\n");
    hwnd = CreateWindow(
        wc.lpszClassName,
        "Pure C Software Rendering (StretchDIBits)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    if (!hwnd)
    {
        fprintf(stderr, "[FAIL] CreateWindow == NULL\n");
        return 1;
    }
    printf("[ OK ] hwnd=%p\n", (void*)hwnd);

    printf("[STEP] ShowWindow / UpdateWindow\n");
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    printf("[STEP] malloc pixel buffer\n");
    bufferSize = (size_t)SCREEN_WIDTH * (size_t)SCREEN_HEIGHT * sizeof(DWORD);
    g_pixelBuffer = (DWORD*)malloc(bufferSize);
    if (!g_pixelBuffer)
    {
        fprintf(stderr, "[FAIL] malloc(%zu) == NULL\n", bufferSize);
        DestroyWindow(hwnd);
        return 1;
    }
    printf("[ OK ] pixelBuffer=%p, size=%zu bytes\n", (void*)g_pixelBuffer, bufferSize);

    memset(g_pixelBuffer, 0, bufferSize);

    printf("[STEP] setup BITMAPINFO\n");
    memset(&g_bmi, 0, sizeof(BITMAPINFO));
    g_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    g_bmi.bmiHeader.biWidth = SCREEN_WIDTH;
    g_bmi.bmiHeader.biHeight = -SCREEN_HEIGHT;
    g_bmi.bmiHeader.biPlanes = 1;
    g_bmi.bmiHeader.biBitCount = 32;
    g_bmi.bmiHeader.biCompression = BI_RGB;
    g_bmi.bmiHeader.biSizeImage = (DWORD)bufferSize;
    printf("[ OK ] biWidth=%d biHeight=%d biBitCount=%d\n",
        (int)g_bmi.bmiHeader.biWidth,
        (int)g_bmi.bmiHeader.biHeight,
        (int)g_bmi.bmiHeader.biBitCount);

    msg.message = WM_NULL;
    running = TRUE;

    printf("[STEP] enter main loop\n");

    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            printf("[MAIN] PeekMessage -> msg=0x%04X (%s) hwnd=%p wParam=%llu lParam=%lld\n",
                (unsigned)msg.message,
                MsgName(msg.message),
                (void*)msg.hwnd,
                (unsigned long long)msg.wParam,
                (long long)msg.lParam);

            if (msg.message == WM_QUIT)
            {
                printf("[MAIN] WM_QUIT received, exiting loop\n");
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

        ++g_frameCounter;
        g_timeCounter += 0.05f;

        printf("[MAIN] frame %u: RenderFrame(t=%.3f)\n", g_frameCounter, g_timeCounter);

        RenderFrame(g_pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, g_timeCounter);

        printf("[MAIN] frame %u: InvalidateRect + UpdateWindow\n", g_frameCounter);

        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }

    printf("[STEP] cleanup\n");

    free(g_pixelBuffer);
    g_pixelBuffer = NULL;

    printf("[DONE]\n");
    return 0;
}
