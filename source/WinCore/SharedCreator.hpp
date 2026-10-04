// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SharedCreator_hpp
#define WinCore_SharedCreator_hpp

#include <WinCore/Kernel32.h>
#include <WinCore/Shared.hpp>

class SharedCreator
{
public:
	Shared* Create(LPCSTR lpLibFileName);
private:
};

#endif
