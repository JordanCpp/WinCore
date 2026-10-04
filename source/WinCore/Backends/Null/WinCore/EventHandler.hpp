// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Null_EventHandler_hpp
#define WinCore_Null_EventHandler_hpp

#include <WinCore/Windows.h>

class EventHandler
{
public:
	EventHandler();
	bool IsRunning();
	void StopEvents();
	bool GetEvent(MSG& msg);
	bool WaitEvent(MSG& msg);
private:
	bool _running;
};

#endif
