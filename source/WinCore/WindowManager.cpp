// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/WindowManager.hpp>

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
	for (container::iterator i = _windows.begin(); i != _windows.end(); i++)
	{
		delete i->second;
	}
}

void WindowManager::Append(HWND handle, Window* window)
{
	_windows.insert(std::make_pair(handle, window));
}

Window* WindowManager::Find(HWND hwnd)
{
	container::iterator i = _windows.find(hwnd);

	if (i != _windows.end())
	{
		return i->second;
	}

	return NULL;
}

Window* WindowManager::FindNative(void* native)
{
	for (container::iterator i = _windows.begin(); i != _windows.end(); i++)
	{
		if (i->second->Native() == native)
		{
			return i->second;
		}
	}

	return NULL;
}
