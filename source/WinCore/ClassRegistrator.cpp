// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

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
		window.lpszClassName = wndClass->lpszClassName ? wndClass->lpszClassName : "";
		window.lpszMenuName  = wndClass->lpszMenuName  ? wndClass->lpszMenuName  : "";

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
