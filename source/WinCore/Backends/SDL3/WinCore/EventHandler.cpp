/*
Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp)

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, see <https://gnu.org>.
*/

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
