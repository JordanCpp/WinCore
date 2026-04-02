/*
Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp)

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, see <https://gnu.org>.
*/

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
