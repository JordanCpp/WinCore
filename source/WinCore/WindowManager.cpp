// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/WindowManager.hpp>

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
	_windows.clear();
}

void WindowManager::Append(HWND handle, Window* window)
{
	_windows.insert(std::make_pair(handle, window));
}

void WindowManager::Remove(HWND handle)
{
	container::iterator i = _windows.find(handle);

	if (i != _windows.end())
	{
		_windows.erase(i);
	}
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
	if (!native)
	{
		return NULL;
	}

	for (container::iterator i = _windows.begin(); i != _windows.end(); i++)
	{
		if (i->second && i->second->Native() == native)
		{
			return i->second;
		}
	}

	return NULL;
}
