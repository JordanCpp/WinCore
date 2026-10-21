/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <WinCore/Application.hpp>

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

int main()
{
    RunMessageQueueTests();
    RunClassRegistratorTests();
    RunApplicationTests();

	return 0;
}
