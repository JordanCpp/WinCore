// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_WindowManager_hpp
#define WinCore_WindowManager_hpp

#include <map>
#include <WinCore/Window.hpp>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();
	void Append(HWND handle, Window* window);
	void Remove(HWND handle);
	Window* Find(HWND hwnd);
	Window* FindNative(void* native);
	BOOL Destroy(HWND handle);
private:
	typedef std::map<HWND, Window*> container;
	container _windows;
};

#endif
