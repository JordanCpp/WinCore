/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <WinCore/Windows.h>

static BOOL g_fullscreen = FALSE;

static void ToggleFullscreen(HWND hwnd)
{
    if (g_fullscreen)
    {
        ChangeDisplaySettingsA(NULL, CDS_RESET);
        g_fullscreen = FALSE;
    }
    else
    {
        DEVMODEA dm;
        memset(&dm, 0, sizeof(dm));
        dm.dmSize = sizeof(DEVMODEA);

        if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm))
        {
            dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

            if (ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
            {
                g_fullscreen = TRUE;
            }
        }
    }

    (void)hwnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        printf("Window created. F = fullscreen, Esc = windowed, close = exit.\n");
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            if (g_fullscreen)
            {
                ToggleFullscreen(hwnd);
            }
            else
            {
                DestroyWindow(hwnd);
            }
            return 0;
        }

        if (wParam == 'F' || wParam == 'f')
        {
            ToggleFullscreen(hwnd);
            return 0;
        }
        break;

    case WM_CLOSE:
        if (g_fullscreen)
        {
            ChangeDisplaySettingsA(NULL, CDS_RESET);
            g_fullscreen = FALSE;
        }
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main(void)
{
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "WinCoreFullscreenDemo";

    if (!RegisterClassA(&wc))
    {
        printf("RegisterClassA failed\n");
        return 1;
    }

    HWND hwnd = CreateWindowExA(0,
        wc.lpszClassName,
        "WinCore Fullscreen Demo",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, NULL, NULL);

    if (!hwnd)
    {
        printf("CreateWindowExA failed\n");
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}