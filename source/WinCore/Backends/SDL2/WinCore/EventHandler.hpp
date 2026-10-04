// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL2_EventHandler_hpp
#define WinCore_SDL2_EventHandler_hpp

#include <queue>
#include <WinCore/Windows.h>
#include <WinCore/Backends/SDL2/WinCore/EventTranslator.hpp>

class EventHandler
{
public:
	EventHandler();
	bool IsRunning();
	void StopEvents();
	void PushMessage(const MSG& msg);
	bool GetEvent(MSG& msg);
	bool WaitEvent(MSG& msg);
private:
	bool            _running;
	EventTranslator _translator;
	std::queue<MSG> _manualEvents;
};

#endif
