// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

HMODULE LoadLibraryA(LPCSTR lpLibFileName)
{
	return MainApplication().LoadLibraryAImpl(lpLibFileName);
}

BOOL FreeLibrary(HMODULE hLibModule)
{
	return MainApplication().FreeLibraryImpl(hLibModule);
}

FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	return MainApplication().GetProcAddressImpl(hModule, lpProcName);
}

DWORD GetTickCount()
{
	return MainApplication()._ticks.GetTickCount();
}

void Sleep(DWORD dwMilliseconds)
{
	MainApplication()._ticks.Sleep(dwMilliseconds);
}
