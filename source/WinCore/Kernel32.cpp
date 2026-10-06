// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

HMODULE LoadLibraryA(LPCSTR lpLibFileName)
{
	return (HMODULE)MainApplication()._sharedCreator.Create(lpLibFileName);
}

BOOL FreeLibrary(HMODULE hLibModule)
{
	Shared* shared = (Shared*)(hLibModule);

	shared->Unload();

	return TRUE;
}

FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	Shared* shared = (Shared*)(hModule);

	return (FARPROC)shared->GetFunction(lpProcName);
}

DWORD GetTickCount()
{
	return MainApplication()._ticks.GetTickCount();
}

void Sleep(DWORD dwMilliseconds)
{
	MainApplication()._ticks.Sleep(dwMilliseconds);
}
