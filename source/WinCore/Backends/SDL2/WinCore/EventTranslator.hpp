// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL2_EventTranslator_hpp
#define WinCore_SDL2_EventTranslator_hpp

#include <SDL_events.h>
#include <SDL_keycode.h>
#include <WinCore/WindowManager.hpp>

class EventTranslator
{
public:
    EventTranslator(WindowManager& windowManager);
    void Translate(const SDL_Event& sdlEvent, MSG& winMsg);
    WPARAM TranslateKey(SDL_Keycode sdlKey);
private:
    WindowManager& _windowManager;
};

#endif
