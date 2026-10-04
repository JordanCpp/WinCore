// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/SharedCreator.hpp>

Shared* SharedCreator::Create(LPCSTR lpLibFileName)
{
	Shared* shared = new Shared;

	shared->Load(lpLibFileName);

	return shared;
}
