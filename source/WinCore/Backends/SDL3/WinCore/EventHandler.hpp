// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_EventHandler_hpp
#define WinCore_SDL3_EventHandler_hpp

#include <WinCore/MessageQueue.hpp>
#include <WinCore/Backends/SDL3/WinCore/EventTranslator.hpp>

class EventHandler
{
public:
    void PumpEvents();
    bool WaitAndPush();
    MessageQueue& Messages();
private:
	MessageQueue    _queue;
	EventTranslator _translator;
};

#endif
