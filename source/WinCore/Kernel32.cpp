// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

HMODULE LoadLibraryA(LPCSTR lpLibFileName)
{
    HMODULE hModule = MainApplication()._moduleManager.Create(lpLibFileName);

    if (!hModule)
    {
        MainApplication().SetLastError(ERROR_MOD_NOT_FOUND);
        return NULL;
    }

    MainApplication().SetLastError(ERROR_SUCCESS);
    return hModule;
}

BOOL FreeLibrary(HMODULE hLibModule)
{
    if (!hLibModule)
    {
        MainApplication().SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (!MainApplication()._moduleManager.Destroy(hLibModule))
    {
        MainApplication().SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    MainApplication().SetLastError(ERROR_SUCCESS);
    return TRUE;
}

FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    if (!hModule)
    {
        MainApplication().SetLastError(ERROR_MOD_NOT_FOUND);
        return NULL;
    }

    if (!lpProcName)
    {
        MainApplication().SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    Shared* shared = MainApplication()._moduleManager.Find(hModule);

    if (!shared)
    {
        MainApplication().SetLastError(ERROR_INVALID_HANDLE);
        return NULL;
    }

    void* fn = shared->GetFunction(lpProcName);

    if (!fn)
    {
        MainApplication().SetLastError(ERROR_PROC_NOT_FOUND);
        return NULL;
    }

    MainApplication().SetLastError(ERROR_SUCCESS);
    return (FARPROC)fn;
}

DWORD GetTickCount()
{
    return MainApplication()._ticks.GetTickCount();
}

void Sleep(DWORD dwMilliseconds)
{
    MainApplication()._ticks.Sleep(dwMilliseconds);
}

DWORD GetLastError()
{
    return MainApplication().GetLastError();
}

void SetLastError(DWORD dwErrCode)
{
    MainApplication().SetLastError(dwErrCode);
}
