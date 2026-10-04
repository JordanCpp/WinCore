// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_EventHandler_hpp
#define WinCore_SDL3_EventHandler_hpp

#include <deque>
#include <WinCore/Windows.h>

class EventHandler
{
public:
	EventHandler();
	bool IsRunning();
	void StopEvents();
	bool GetEvent(MSG& msg);
	bool WaitEvent(MSG& msg);
	void Pump(std::deque<MSG> messages);
private:
	bool _running;
};

#endif
