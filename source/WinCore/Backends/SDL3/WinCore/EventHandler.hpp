// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_EventHandler_hpp
#define WinCore_SDL3_EventHandler_hpp

#include <deque>
#include <WinCore/Windows.h>
#include <WinCore/Backends/SDL3/WinCore/EventTranslator.hpp>

class EventHandler
{
public:
	EventHandler();
	bool IsRunning();
	void StopEvents();
	void PushMessage(const MSG& msg);
	bool GetEvent(MSG& msg, bool bRemove);
	bool WaitEvent(MSG& msg);
private:
	bool            _running;
	EventTranslator _translator;
	std::deque<MSG> _manualEvents;
};

#endif
