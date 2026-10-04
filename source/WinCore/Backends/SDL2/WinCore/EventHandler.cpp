// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL.h>
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

void EventHandler::PushMessage(const MSG& msg)
{
	_manualEvents.push_back(msg);
}

bool EventHandler::GetEvent(MSG& msg, bool bRemove)
{
	if (!_manualEvents.empty())
	{
		msg = _manualEvents.front();

		if (bRemove)
		{
			_manualEvents.pop_front();
		}

		return true;
	}

	SDL_Event event = { 0 };

	if (SDL_PollEvent(&event))
	{
		_translator.Translate(event, msg);

		if (!bRemove)
		{
			_manualEvents.push_front(msg);
		}

		return true;
	}

	return false;
}

bool EventHandler::WaitEvent(MSG& msg)
{
	if (!_manualEvents.empty())
	{
		msg = _manualEvents.front();
		_manualEvents.pop_front();

		return true;
	}

	SDL_Event event = { 0 };

	if (SDL_WaitEvent(&event))
	{
		_translator.Translate(event, msg);

		return true;
	}

	return false;
}
