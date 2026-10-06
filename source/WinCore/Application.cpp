// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

static Application _application;

Application::Application()
{
}

Application::~Application()
{
}

Application& MainApplication()
{
	return _application;
}
