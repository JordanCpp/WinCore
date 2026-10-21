// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

Application::Application() :
    _moduleManager(_sharedCreator),
    _eventHandler(_windowManager),
    _lastError(0)
{
}

Application::~Application()
{
}

void Application::SetLastError(DWORD err)
{
    _lastError = err;
}

DWORD Application::GetLastError() const
{
    return _lastError;
}

Application& MainApplication()
{
    static Application app;
    return app;
}
