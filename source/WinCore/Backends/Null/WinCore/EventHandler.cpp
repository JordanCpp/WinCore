// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/EventHandler.hpp>

EventHandler::EventHandler() :
	_running(true)
{
}

bool EventHandler::IsRunning()
{
	return _running;
}

void EventHandler::StopEvents()
{
	_running = false;
}

bool EventHandler::GetEvent(MSG& msg)
{
	return false;
}

bool EventHandler::WaitEvent(MSG& msg)
{
	return false;
}
