// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <SDL3/SDL_events.h>
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
	SDL_Event event = { 0 };

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			msg.message = WM_DESTROY;
		}

		return true;
	}

	return false;
}

bool EventHandler::WaitEvent(MSG& msg)
{
	SDL_Event event = { 0 };

	if (SDL_WaitEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			msg.message = WM_DESTROY;
		}

		return true;
	}

	return false;
}

void EventHandler::Pump(std::deque<MSG> messages)
{
	SDL_Event event = { 0 };

	while (SDL_PollEvent(&event))
	{
		MSG msg = { 0 };

		msg.hwnd = (HWND)SDL_GetWindowFromID(event.window.windowID);

		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			msg.message = WM_DESTROY;
			messages.push_back(msg);
			break;
		case SDL_EVENT_WINDOW_EXPOSED:
			msg.message = WM_PAINT;
			messages.push_back(msg);
			break;
		}
	}
}
