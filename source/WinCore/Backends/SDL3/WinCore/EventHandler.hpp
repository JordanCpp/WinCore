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

#ifndef WinCore_SDL3_EventHandler_hpp
#define WinCore_SDL3_EventHandler_hpp

#include <WinCore/Windows.h>

class EventHandler
{
public:
	EventHandler();
	bool IsRunning();
	void StopEvents();
	bool GetEvent(MSG& msg);
private:
	bool _running;
};

#endif
