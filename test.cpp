/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <WinCore/Application.hpp>
#include <WinCore/Windows.h>

void TestEqual(bool condition, const char* description, const char* function, const char* file, int line)
{
    if (!condition)
    {
        printf("Test failed: %s, function: %s, file: %s, line: %d\n", description, function, file, line);
    }
}

#define TEST_EQUAL(x) TestEqual((x), #x, __FUNCTION__, __FILE__, __LINE__)

static MSG MakeMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MSG m;
    m.hwnd = hwnd;
    m.message = message;
    m.wParam = wParam;
    m.lParam = lParam;
    m.time = 0;
    m.pt.x = 0;
    m.pt.y = 0;
    return m;
}

void Test_MessageQueue_PushAndPop()
{
    MessageQueue q;
    q.Push(MakeMsg((HWND)0x1, WM_KEYDOWN, 0x41, 0));

    TEST_EQUAL(!q.Empty());
    TEST_EQUAL(q.Size() == 1);

    MSG m;
    TEST_EQUAL(q.Pop(m));
    TEST_EQUAL(m.message == WM_KEYDOWN);
    TEST_EQUAL(m.wParam == 0x41);
    TEST_EQUAL(q.Empty());
}

void Test_MessageQueue_PopEmpty()
{
    MessageQueue q;
    MSG m;
    TEST_EQUAL(!q.Pop(m));
}

void Test_MessageQueue_PostQuit()
{
    MessageQueue q;
    TEST_EQUAL(q.IsRunning());

    q.PostQuit(7);

    TEST_EQUAL(!q.IsRunning());
    TEST_EQUAL(q.Size() == 1);

    MSG m;
    TEST_EQUAL(q.Pop(m));
    TEST_EQUAL(m.message == WM_QUIT);
    TEST_EQUAL(m.wParam == 7);
}

void Test_MessageQueue_PostQuitIdempotent()
{
    MessageQueue q;
    q.PostQuit(1);
    q.PostQuit(2);
    q.PostQuit(3);

    TEST_EQUAL(q.Size() == 1);

    MSG m;
    TEST_EQUAL(q.Pop(m));
    TEST_EQUAL(m.wParam == 1);
}

void Test_MessageQueue_PushQuit()
{
    MessageQueue q;
    q.Push(MakeMsg(NULL, WM_QUIT, 42, 0));
    q.Push(MakeMsg(NULL, WM_QUIT, 43, 0));

    TEST_EQUAL(q.Size() == 1);

    MSG m;
    TEST_EQUAL(q.Pop(m));
    TEST_EQUAL(m.message == WM_QUIT);
    TEST_EQUAL(m.wParam == 42);
}

void Test_MessageQueue_FilteredByHwnd()
{
    MessageQueue q;
    q.Push(MakeMsg((HWND)0x1, WM_KEYDOWN, 1, 0));
    q.Push(MakeMsg((HWND)0x2, WM_KEYDOWN, 2, 0));
    q.Push(MakeMsg((HWND)0x1, WM_KEYUP, 3, 0));

    MSG m;
    TEST_EQUAL(q.PeekFiltered(m, true, (HWND)0x1, WM_KEYDOWN, WM_KEYUP));
    TEST_EQUAL(m.wParam == 1);

    TEST_EQUAL(q.PeekFiltered(m, true, (HWND)0x1, WM_KEYDOWN, WM_KEYUP));
    TEST_EQUAL(m.wParam == 3);

    TEST_EQUAL(!q.PeekFiltered(m, true, (HWND)0x1, WM_KEYDOWN, WM_KEYUP));
}

void Test_MessageQueue_FilteredByRange()
{
    MessageQueue q;
    q.Push(MakeMsg(NULL, 100, 0, 0));
    q.Push(MakeMsg(NULL, 200, 0, 0));
    q.Push(MakeMsg(NULL, 300, 0, 0));

    MSG m;
    TEST_EQUAL(q.PeekFiltered(m, true, NULL, 150, 250));
    TEST_EQUAL(m.message == 200);
}

void Test_MessageQueue_QuitHasPriority()
{
    MessageQueue q;
    q.Push(MakeMsg((HWND)0x1, WM_KEYDOWN, 1, 0));
    q.Push(MakeMsg(NULL, WM_QUIT, 9, 0));

    MSG m;
    TEST_EQUAL(q.PeekFiltered(m, true, (HWND)0x1, WM_KEYDOWN, WM_KEYUP));
    TEST_EQUAL(m.message == WM_QUIT);
    TEST_EQUAL(m.wParam == 9);
}

void Test_MessageQueue_EmptyOnStart()
{
    MessageQueue q;
    TEST_EQUAL(q.Empty());
    TEST_EQUAL(q.Size() == 0);
    TEST_EQUAL(q.IsRunning());
}

void Test_MessageQueue_QuitRespectsOrder()
{
    MessageQueue q;
    q.Push(MakeMsg((HWND)0x1, WM_KEYDOWN, 1, 0));
    q.Push(MakeMsg(NULL, WM_QUIT, 9, 0));

    MSG m;

    TEST_EQUAL(q.PeekFiltered(m, true, (HWND)0x1, WM_KEYDOWN, WM_KEYUP));
    TEST_EQUAL(m.message == WM_KEYDOWN);
    TEST_EQUAL(m.wParam == 1);

    TEST_EQUAL(q.PeekFiltered(m, true, (HWND)0x1, WM_KEYDOWN, WM_KEYUP));
    TEST_EQUAL(m.message == WM_QUIT);
    TEST_EQUAL(m.wParam == 9);
}

void Test_MessageQueue_QuitWithoutMatchingFilter()
{
    MessageQueue q;
    q.Push(MakeMsg(NULL, WM_QUIT, 9, 0));
    q.Push(MakeMsg((HWND)0x1, WM_KEYDOWN, 1, 0));

    MSG m;
    TEST_EQUAL(q.PeekFiltered(m, true, (HWND)0x2, WM_KEYDOWN, WM_KEYUP));
    TEST_EQUAL(m.message == WM_QUIT);
    TEST_EQUAL(m.wParam == 9);

    TEST_EQUAL(q.PeekFiltered(m, true, (HWND)0x1, WM_KEYDOWN, WM_KEYUP));
    TEST_EQUAL(m.message == WM_KEYDOWN);
}

void RunMessageQueueTests()
{
    Test_MessageQueue_EmptyOnStart();
    Test_MessageQueue_PushAndPop();
    Test_MessageQueue_PopEmpty();
    Test_MessageQueue_PostQuit();
    Test_MessageQueue_PostQuitIdempotent();
    Test_MessageQueue_PushQuit();
    Test_MessageQueue_FilteredByHwnd();
    Test_MessageQueue_FilteredByRange();
    Test_MessageQueue_QuitRespectsOrder();
    Test_MessageQueue_QuitWithoutMatchingFilter();
}

static LRESULT CALLBACK DummyProc(HWND, UINT, WPARAM, LPARAM)
{
    return 0;
}

static WNDCLASSA MakeClass(const char* name)
{
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = DummyProc;
    wc.lpszClassName = name;
    return wc;
}

void Test_ClassRegistrator_AppendSuccess()
{
    ClassRegistrator reg;
    WNDCLASSA wc = MakeClass("TestClassA");

    ATOM a = reg.Append(&wc);
    TEST_EQUAL(a != 0);
}

void Test_ClassRegistrator_AppendDuplicate()
{
    ClassRegistrator reg;
    WNDCLASSA wc = MakeClass("TestClassB");

    ATOM a1 = reg.Append(&wc);
    ATOM a2 = reg.Append(&wc);

    TEST_EQUAL(a1 != 0);
    TEST_EQUAL(a2 == 0);
}

void Test_ClassRegistrator_AppendNull()
{
    ClassRegistrator reg;
    TEST_EQUAL(reg.Append(NULL) == 0);
}

void Test_ClassRegistrator_AppendNoProc()
{
    ClassRegistrator reg;
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpszClassName = "NoProc";

    TEST_EQUAL(reg.Append(&wc) == 0);
}

void Test_ClassRegistrator_AppendNoName()
{
    ClassRegistrator reg;
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = DummyProc;

    TEST_EQUAL(reg.Append(&wc) == 0);
}

void Test_ClassRegistrator_Find()
{
    ClassRegistrator reg;
    WNDCLASSA wc = MakeClass("FindMe");
    ATOM a = reg.Append(&wc);

    WindowClassA out;
    TEST_EQUAL(reg.Find("FindMe", out));
    TEST_EQUAL(out.atom == a);
    TEST_EQUAL(out.lpfnWndProc == (WNDPROC)DummyProc);
    TEST_EQUAL(out.lpszClassName == "FindMe");
}

void Test_ClassRegistrator_FindMissing()
{
    ClassRegistrator reg;
    WindowClassA out;
    TEST_EQUAL(!reg.Find("Nope", out));
}

void Test_ClassRegistrator_AtomsUnique()
{
    ClassRegistrator reg;
    WNDCLASSA wc1 = MakeClass("C1");
    WNDCLASSA wc2 = MakeClass("C2");
    WNDCLASSA wc3 = MakeClass("C3");

    ATOM a1 = reg.Append(&wc1);
    ATOM a2 = reg.Append(&wc2);
    ATOM a3 = reg.Append(&wc3);

    TEST_EQUAL(a1 != a2);
    TEST_EQUAL(a2 != a3);
    TEST_EQUAL(a1 != a3);
}

void Test_ClassRegistrator_AtomRange()
{
    ClassRegistrator reg;
    WNDCLASSA wc = MakeClass("Range");
    ATOM a = reg.Append(&wc);

    TEST_EQUAL(a >= 0xC000);
    TEST_EQUAL(a <= 0xFFFF);
}

void Test_ClassRegistrator_AtomStableAfterFind()
{
    ClassRegistrator reg;
    WNDCLASSA wc = MakeClass("Stable");
    ATOM a = reg.Append(&wc);

    WindowClassA out;
    TEST_EQUAL(reg.Find("Stable", out));
    TEST_EQUAL(out.atom == a);
}

void RunClassRegistratorTests()
{
    Test_ClassRegistrator_AppendSuccess();
    Test_ClassRegistrator_AppendDuplicate();
    Test_ClassRegistrator_AppendNull();
    Test_ClassRegistrator_AppendNoProc();
    Test_ClassRegistrator_AppendNoName();
    Test_ClassRegistrator_Find();
    Test_ClassRegistrator_FindMissing();
    Test_ClassRegistrator_AtomsUnique();
    Test_ClassRegistrator_AtomRange();
    Test_ClassRegistrator_AtomStableAfterFind();
}

void Test_Application_SetGetLastError()
{
    MainApplication().SetLastError(ERROR_SUCCESS);
    TEST_EQUAL(MainApplication().GetLastError() == ERROR_SUCCESS);

    MainApplication().SetLastError(ERROR_CLASS_ALREADY_EXISTS);
    TEST_EQUAL(MainApplication().GetLastError() == ERROR_CLASS_ALREADY_EXISTS);

    MainApplication().SetLastError(0);
}

void Test_Application_SingletonStable()
{
    Application* a = &MainApplication();
    Application* b = &MainApplication();
    TEST_EQUAL(a == b);
}

void RunApplicationTests()
{
    Test_Application_SetGetLastError();
    Test_Application_SingletonStable();
}

static LRESULT CALLBACK GdiTestProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)  return 0;
    if (msg == WM_DESTROY) return 0;
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

static HWND CreateGdiTestWindow(const char* className)
{
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = GdiTestProc;
    wc.lpszClassName = className;

    if (!RegisterClassA(&wc))
    {
        return NULL;
    }

    return CreateWindowExA(0, className, "GDI Test",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        320, 240, NULL, NULL, NULL, NULL);
}

static DWORD* MakeSolidPixels(int w, int h, DWORD color)
{
    DWORD* pixels = (DWORD*)malloc((size_t)w * h * sizeof(DWORD));
    if (!pixels) return NULL;

    for (int i = 0; i < w * h; ++i)
    {
        pixels[i] = color;
    }

    return pixels;
}

static void MakeBmi(BITMAPINFO* bmi, int w, int h, int biHeight)
{
    memset(bmi, 0, sizeof(BITMAPINFO));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = w;
    bmi->bmiHeader.biHeight = biHeight;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 32;
    bmi->bmiHeader.biCompression = BI_RGB;
    (void)h;
}

void Test_Gdi_SetDIBits_TopDown_GetDC()
{
    HWND h = CreateGdiTestWindow("Gdi_TD_GetDC");
    TEST_EQUAL(h != NULL);

    const int W = 64, H = 64;
    DWORD* pixels = MakeSolidPixels(W, H, 0x00FF0000);
    TEST_EQUAL(pixels != NULL);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    HDC hdc = GetDC(h);
    TEST_EQUAL(hdc != NULL);

    int result = SetDIBitsToDevice(hdc, 0, 0, W, H,
        0, 0, 0, H,
        pixels, &bmi, DIB_RGB_COLORS);

    TEST_EQUAL(result == H);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_BottomUp_GetDC()
{
    HWND h = CreateGdiTestWindow("Gdi_BU_GetDC");
    TEST_EQUAL(h != NULL);

    const int W = 64, H = 64;
    DWORD* pixels = MakeSolidPixels(W, H, 0x0000FF00);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, H);

    HDC hdc = GetDC(h);

    int result = SetDIBitsToDevice(hdc, 0, 0, W, H,
        0, 0, 0, H,
        pixels, &bmi, DIB_RGB_COLORS);

    TEST_EQUAL(result == H);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_InPaint()
{
    HWND h = CreateGdiTestWindow("Gdi_InPaint");
    TEST_EQUAL(h != NULL);

    const int W = 32, H = 32;
    DWORD* pixels = MakeSolidPixels(W, H, 0x000000FF);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(h, &ps);
    TEST_EQUAL(hdc != NULL);

    int result = SetDIBitsToDevice(hdc, 0, 0, W, H,
        0, 0, 0, H,
        pixels, &bmi, DIB_RGB_COLORS);
    TEST_EQUAL(result == H);

    TEST_EQUAL(EndPaint(h, &ps));

    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_MultipleSprites()
{
    HWND h = CreateGdiTestWindow("Gdi_MultiSprite");
    TEST_EQUAL(h != NULL);

    const int W = 16, H = 16;
    DWORD* red = MakeSolidPixels(W, H, 0x00FF0000);
    DWORD* green = MakeSolidPixels(W, H, 0x0000FF00);
    DWORD* blue = MakeSolidPixels(W, H, 0x000000FF);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(h, &ps);
    TEST_EQUAL(hdc != NULL);

    TEST_EQUAL(SetDIBitsToDevice(hdc, 0, 0, W, H, 0, 0, 0, H, red, &bmi, DIB_RGB_COLORS) == H);
    TEST_EQUAL(SetDIBitsToDevice(hdc, 32, 0, W, H, 0, 0, 0, H, green, &bmi, DIB_RGB_COLORS) == H);
    TEST_EQUAL(SetDIBitsToDevice(hdc, 64, 0, W, H, 0, 0, 0, H, blue, &bmi, DIB_RGB_COLORS) == H);

    TEST_EQUAL(EndPaint(h, &ps));

    free(red);
    free(green);
    free(blue);
    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_Banding()
{
    HWND h = CreateGdiTestWindow("Gdi_Banding");
    TEST_EQUAL(h != NULL);

    const int W = 32, FULL_H = 64;

    DWORD* full = MakeSolidPixels(W, FULL_H, 0x00FFFFFF);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, FULL_H, -FULL_H);

    HDC hdc = GetDC(h);

    for (UINT start = 0; start < FULL_H; start += 16)
    {
        const DWORD* strip = full + (size_t)start * W;

        int result = SetDIBitsToDevice(hdc, 0, (int)start, W, 16, 0, start, start, 16, strip, &bmi, DIB_RGB_COLORS);

        TEST_EQUAL(result == 16);
    }

    ReleaseDC(h, hdc);
    free(full);
    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_ZeroScanLines()
{
    HWND h = CreateGdiTestWindow("Gdi_ZeroScan");
    TEST_EQUAL(h != NULL);

    const int W = 16, H = 16;
    DWORD* pixels = MakeSolidPixels(W, H, 0x00FFFF00);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    HDC hdc = GetDC(h);

    int result = SetDIBitsToDevice(hdc, 0, 0, W, H, 0, 0, 0, 0, pixels, &bmi, DIB_RGB_COLORS);

    TEST_EQUAL(result == H);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_StretchDIBits_Upscale()
{
    HWND h = CreateGdiTestWindow("Gdi_StretchUp");
    TEST_EQUAL(h != NULL);

    const int W = 32, H = 32;
    DWORD* pixels = MakeSolidPixels(W, H, 0x0000FF00);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, H);

    HDC hdc = GetDC(h);

    int result = StretchDIBits(hdc, 0, 0, 128, 128, 0, 0, W, H, pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);

    TEST_EQUAL(result == H);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_StretchDIBits_FlipY()
{
    HWND h = CreateGdiTestWindow("Gdi_FlipY");
    TEST_EQUAL(h != NULL);

    const int W = 16, H = 16;
    DWORD* pixels = MakeSolidPixels(W, H, 0x00FF00FF);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    HDC hdc = GetDC(h);

    int result = StretchDIBits(hdc,
        0, H, W, -H,
        0, 0, W, H,
        pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);

    TEST_EQUAL(result == H);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_StretchDIBits_FlipX()
{
    HWND h = CreateGdiTestWindow("Gdi_FlipX");
    TEST_EQUAL(h != NULL);

    const int W = 16, H = 16;
    DWORD* pixels = MakeSolidPixels(W, H, 0x0000FFFF);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    HDC hdc = GetDC(h);

    int result = StretchDIBits(hdc,
        W, 0, -W, H,
        0, 0, W, H,
        pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);

    TEST_EQUAL(result == H);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_StretchDIBits_FlipXY()
{
    HWND h = CreateGdiTestWindow("Gdi_FlipXY");
    TEST_EQUAL(h != NULL);

    const int W = 16, H = 16;
    DWORD* pixels = MakeSolidPixels(W, H, 0x00808080);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    HDC hdc = GetDC(h);

    int result = StretchDIBits(hdc,
        W, H, -W, -H,
        0, 0, W, H,
        pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);

    TEST_EQUAL(result == H);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_Rejects16Bit()
{
    HWND h = CreateGdiTestWindow("Gdi_16Bit");
    TEST_EQUAL(h != NULL);

    const int W = 16, H = 16;
    WORD* pixels = (WORD*)malloc((size_t)W * H * sizeof(WORD));
    memset(pixels, 0, (size_t)W * H * sizeof(WORD));

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);
    bmi.bmiHeader.biBitCount = 16;

    HDC hdc = GetDC(h);

    int result = SetDIBitsToDevice(hdc, 0, 0, W, H,
        0, 0, 0, H,
        pixels, &bmi, DIB_RGB_COLORS);

    TEST_EQUAL(result == 0);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_StartScanOutOfRange()
{
    HWND h = CreateGdiTestWindow("Gdi_ScanOOR");
    TEST_EQUAL(h != NULL);

    const int W = 16, H = 16;
    DWORD* pixels = MakeSolidPixels(W, H, 0x00FFFFFF);

    BITMAPINFO bmi;
    MakeBmi(&bmi, W, H, -H);

    HDC hdc = GetDC(h);

    int result = SetDIBitsToDevice(hdc, 0, 0, W, H,
        0, 0, H, H,
        pixels, &bmi, DIB_RGB_COLORS);

    TEST_EQUAL(result == 0);

    ReleaseDC(h, hdc);
    free(pixels);
    DestroyWindow(h);
}

void Test_Gdi_InvalidateRect_NoDedup()
{
    HWND h = CreateGdiTestWindow("Gdi_Invalidate");
    TEST_EQUAL(h != NULL);

    MSG m;
    while (PeekMessageA(&m, NULL, 0, 0, PM_REMOVE)) {}

    TEST_EQUAL(InvalidateRect(h, NULL, FALSE));
    TEST_EQUAL(InvalidateRect(h, NULL, FALSE));

    TEST_EQUAL(PeekMessageA(&m, h, WM_PAINT, WM_PAINT, PM_REMOVE));
    TEST_EQUAL(m.message == WM_PAINT);

    TEST_EQUAL(PeekMessageA(&m, h, WM_PAINT, WM_PAINT, PM_REMOVE));
    TEST_EQUAL(m.message == WM_PAINT);

    DestroyWindow(h);
}

void Test_Gdi_SetDIBits_NullArgs()
{
    HWND h = CreateGdiTestWindow("Gdi_NullArgs");
    TEST_EQUAL(h != NULL);

    BITMAPINFO bmi;
    MakeBmi(&bmi, 16, 16, -16);

    HDC hdc = GetDC(h);

    TEST_EQUAL(SetDIBitsToDevice(NULL, 0, 0, 16, 16, 0, 0, 0, 16, NULL, &bmi, 0) == 0);
    TEST_EQUAL(SetDIBitsToDevice(hdc, 0, 0, 16, 16, 0, 0, 0, 16, NULL, &bmi, 0) == 0);
    TEST_EQUAL(SetDIBitsToDevice(hdc, 0, 0, 16, 16, 0, 0, 0, 16, "", NULL, 0) == 0);

    ReleaseDC(h, hdc);
    DestroyWindow(h);
}

void RunGdiTests()
{
    Test_Gdi_SetDIBits_TopDown_GetDC();
    Test_Gdi_SetDIBits_BottomUp_GetDC();
    Test_Gdi_SetDIBits_InPaint();
    Test_Gdi_SetDIBits_MultipleSprites();
    Test_Gdi_SetDIBits_Banding();
    Test_Gdi_SetDIBits_ZeroScanLines();
    Test_Gdi_StretchDIBits_Upscale();
    Test_Gdi_StretchDIBits_FlipY();
    Test_Gdi_StretchDIBits_FlipX();
    Test_Gdi_StretchDIBits_FlipXY();
    Test_Gdi_SetDIBits_Rejects16Bit();
    Test_Gdi_SetDIBits_StartScanOutOfRange();
    Test_Gdi_InvalidateRect_NoDedup();
    Test_Gdi_SetDIBits_NullArgs();
}

int main()
{
    RunMessageQueueTests();
    RunClassRegistratorTests();
    RunApplicationTests();
    RunGdiTests();

	return 0;
}
