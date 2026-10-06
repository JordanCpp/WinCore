// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/EventTranslator.hpp>

EventTranslator::EventTranslator(WindowManager& windowManager) :
    _windowManager(windowManager)
{
}

void EventTranslator::Translate(const SDL_Event& sdlEvent, MSG& winMsg)
{
    winMsg.hwnd = NULL;
    winMsg.wParam = 0;
    winMsg.lParam = 0;
    winMsg.time = static_cast<DWORD>(sdlEvent.common.timestamp);
    winMsg.pt.x = 0;
    winMsg.pt.y = 0;

    Uint32 windowID = 0;

    switch (sdlEvent.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        windowID = sdlEvent.key.windowID;
        break;

    case SDL_MOUSEMOTION:
        windowID = sdlEvent.motion.windowID;
        break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        windowID = sdlEvent.button.windowID;
        break;

    default:
        if (sdlEvent.type == SDL_WINDOWEVENT)
        {
            windowID = sdlEvent.window.windowID;
        }
        break;
    }

    if (windowID != 0)
    {
        SDL_Window* sdlWindow = SDL_GetWindowFromID(windowID);
        if (sdlWindow)
        {
            Window* w = _windowManager.FindNative(sdlWindow);
            if (w)
            {
                winMsg.hwnd = reinterpret_cast<HWND>(w);
            }
        }
    }

    switch (sdlEvent.type)
    {
    case SDL_QUIT:
        winMsg.message = WM_QUIT;
        break;

    case SDL_WINDOWEVENT:
        switch (sdlEvent.window.event)
        {
        case SDL_WINDOWEVENT_CLOSE:
            winMsg.message = WM_CLOSE;
            break;

        case SDL_WINDOWEVENT_EXPOSED:
            winMsg.message = WM_PAINT;
            break;

        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            winMsg.message = WM_SIZE;
            winMsg.wParam = 0;
            winMsg.lParam = (static_cast<LPARAM>(sdlEvent.window.data2) << 16) | (static_cast<LPARAM>(sdlEvent.window.data1) & 0xFFFF);
            break;

        default:
            winMsg.message = WM_NULL;
            break;
        }
        break;

    case SDL_KEYDOWN:
        winMsg.message = WM_KEYDOWN;
        winMsg.wParam = TranslateKey(sdlEvent.key.keysym.sym);
        break;

    case SDL_KEYUP:
        winMsg.message = WM_KEYUP;
        winMsg.wParam = TranslateKey(sdlEvent.key.keysym.sym);
        break;

    case SDL_MOUSEMOTION:
        winMsg.message = WM_MOUSEMOVE;
        winMsg.pt.x = static_cast<LONG>(sdlEvent.motion.x);
        winMsg.pt.y = static_cast<LONG>(sdlEvent.motion.y);
        winMsg.lParam = (static_cast<LPARAM>(winMsg.pt.y) << 16) | (static_cast<LPARAM>(winMsg.pt.x) & 0xFFFF);
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (sdlEvent.button.button == SDL_BUTTON_LEFT)        winMsg.message = WM_LBUTTONDOWN;
        else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)  winMsg.message = WM_RBUTTONDOWN;
        else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) winMsg.message = WM_MBUTTONDOWN;
        winMsg.pt.x = static_cast<LONG>(sdlEvent.button.x);
        winMsg.pt.y = static_cast<LONG>(sdlEvent.button.y);
        winMsg.lParam = (static_cast<LPARAM>(winMsg.pt.y) << 16) | (static_cast<LPARAM>(winMsg.pt.x) & 0xFFFF);
        break;

    case SDL_MOUSEBUTTONUP:
        if (sdlEvent.button.button == SDL_BUTTON_LEFT)        winMsg.message = WM_LBUTTONUP;
        else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)  winMsg.message = WM_RBUTTONUP;
        else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) winMsg.message = WM_MBUTTONUP;
        winMsg.pt.x = static_cast<LONG>(sdlEvent.button.x);
        winMsg.pt.y = static_cast<LONG>(sdlEvent.button.y);
        winMsg.lParam = (static_cast<LPARAM>(winMsg.pt.y) << 16) | (static_cast<LPARAM>(winMsg.pt.x) & 0xFFFF);
        break;

    default:
        winMsg.message = WM_NULL;
        break;
    }
}

WPARAM EventTranslator::TranslateKey(SDL_Keycode sdlKey)
{
    if (sdlKey >= SDLK_a && sdlKey <= SDLK_z)
    {
        return 'A' + (sdlKey - SDLK_a);
    }

    if (sdlKey >= SDLK_0 && sdlKey <= SDLK_9)
    {
        return '0' + (sdlKey - SDLK_0);
    }

    switch (sdlKey)
    {
        // Control & System Keys
    case SDLK_ESCAPE:    return VK_ESCAPE;
    case SDLK_RETURN:    return VK_RETURN;
    case SDLK_BACKSPACE: return VK_BACK;
    case SDLK_TAB:       return VK_TAB;
    case SDLK_SPACE:     return VK_SPACE;

        // Modifiers
    case SDLK_LSHIFT:    return VK_LSHIFT;
    case SDLK_RSHIFT:    return VK_RSHIFT;
    case SDLK_LCTRL:     return VK_LCONTROL;
    case SDLK_RCTRL:     return VK_RCONTROL;
    case SDLK_LALT:      return VK_LMENU;
    case SDLK_RALT:      return VK_RMENU;

        // Navigation
    case SDLK_UP:        return VK_UP;
    case SDLK_DOWN:      return VK_DOWN;
    case SDLK_LEFT:      return VK_LEFT;
    case SDLK_RIGHT:     return VK_RIGHT;
    case SDLK_INSERT:    return VK_INSERT;
    case SDLK_DELETE:    return VK_DELETE;
    case SDLK_HOME:      return VK_HOME;
    case SDLK_END:       return VK_END;
    case SDLK_PAGEUP:    return VK_PRIOR;
    case SDLK_PAGEDOWN:  return VK_NEXT;

        // Function keys
    case SDLK_F1:        return VK_F1;
    case SDLK_F2:        return VK_F2;
    case SDLK_F3:        return VK_F3;
    case SDLK_F4:        return VK_F4;
    case SDLK_F5:        return VK_F5;
    case SDLK_F6:        return VK_F6;
    case SDLK_F7:        return VK_F7;
    case SDLK_F8:        return VK_F8;
    case SDLK_F9:        return VK_F9;
    case SDLK_F10:       return VK_F10;
    case SDLK_F11:       return VK_F11;
    case SDLK_F12:       return VK_F12;

        // Numpad
    case SDLK_KP_0:        return VK_NUMPAD0;
    case SDLK_KP_1:        return VK_NUMPAD1;
    case SDLK_KP_2:        return VK_NUMPAD2;
    case SDLK_KP_3:        return VK_NUMPAD3;
    case SDLK_KP_4:        return VK_NUMPAD4;
    case SDLK_KP_5:        return VK_NUMPAD5;
    case SDLK_KP_6:        return VK_NUMPAD6;
    case SDLK_KP_7:        return VK_NUMPAD7;
    case SDLK_KP_8:        return VK_NUMPAD8;
    case SDLK_KP_9:        return VK_NUMPAD9;
    case SDLK_KP_MULTIPLY: return VK_MULTIPLY;
    case SDLK_KP_PLUS:     return VK_ADD;
    case SDLK_KP_MINUS:    return VK_SUBTRACT;
    case SDLK_KP_DIVIDE:   return VK_DIVIDE;
    case SDLK_KP_ENTER:    return VK_RETURN;

    default:               return 0;
    }
}
