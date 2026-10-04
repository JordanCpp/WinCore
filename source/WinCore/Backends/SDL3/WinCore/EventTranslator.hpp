// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_EventTranslator_hpp
#define WinCore_SDL3_EventTranslator_hpp

#include <SDL3/SDL_events.h>
#include <WinCore/Windows.h>

class EventTranslator
{
public:
    void Translate(const SDL_Event& sdlEvent, MSG& winMsg);
    WPARAM TranslateKey(SDL_Keycode sdlKey);
};

#endif
