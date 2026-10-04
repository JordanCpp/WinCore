// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_EventTranslator_hpp
#define WinCore_EventTranslator_hpp

#include <SDL.h>
#include <WinCore/Windows.h>

class EventTranslator
{
public:
    void Translate(const SDL_Event& sdlEvent, MSG& winMsg);
    WPARAM TranslateKey(SDL_Keycode sdlKey);
};

#endif
