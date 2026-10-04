// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_ClassRegistrator_hpp
#define WinCore_ClassRegistrator_hpp

#include <map>
#include <WinCore/WindowClassA.hpp>

class ClassRegistrator
{
public:
	typedef std::map<std::string, WindowClassA> container;
	void Append(const WNDCLASSA* wndClass);
	bool Find(std::string name, WindowClassA& window);
	const container& GetClasses();
private:
	container _classes;
};

#endif
