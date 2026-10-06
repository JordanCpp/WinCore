// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/EventHandler.hpp>

EventHandler::EventHandler(WindowManager& windowManager) :
    _translator(windowManager)
{
}

void EventHandler::PumpEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        MSG msg;
        _translator.Translate(event, msg);
        _queue.Push(msg);
    }
}

bool EventHandler::WaitAndPush()
{
    SDL_Event event;

    if (!SDL_WaitEvent(&event))
    {
        return false;
    }

    MSG msg;
    _translator.Translate(event, msg);
    _queue.Push(msg);
    return true;
}

MessageQueue& EventHandler::Messages()
{ 
    return _queue; 
}
