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

#include <WinCore/ClassRegistrator.hpp>

void ClassRegistrator::Append(const WNDCLASSA* wndClass)
{
	container::iterator i = _classes.find(wndClass->lpszClassName);

	if (i == _classes.end())
	{
		WindowClassA window;

		window.cbClsExtra    = wndClass->cbClsExtra;
		window.cbWndExtra    = wndClass->cbWndExtra;
		window.hbrBackground = wndClass->hbrBackground;
		window.hCursor       = wndClass->hCursor;
		window.hIcon         = wndClass->hIcon;
		window.hInstance     = wndClass->hInstance;
		window.lpfnWndProc   = wndClass->lpfnWndProc;
		window.lpszClassName = wndClass->lpszClassName;
		window.lpszMenuName  = wndClass->lpszMenuName;

		_classes.insert(std::make_pair(wndClass->lpszClassName, window));
	}
}

bool ClassRegistrator::Find(std::string name, WindowClassA& window)
{
	container::iterator i = _classes.find(name);

	if (i != _classes.end())
	{
		window = i->second;

		return true;
	}

	return false;
}

const ClassRegistrator::container& ClassRegistrator::GetClasses()
{
	return _classes;
}
