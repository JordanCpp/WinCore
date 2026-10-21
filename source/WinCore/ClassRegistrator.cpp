// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/ClassRegistrator.hpp>

static const ATOM kAtomMin = 0xC000;
static const ATOM kAtomMax = 0xFFFF;

ClassRegistrator::ClassRegistrator() :
    _nextAtom(kAtomMin)
{
}

ATOM ClassRegistrator::AllocateAtom() const
{
    ATOM candidate = _nextAtom;

    for (unsigned int i = 0; i <= (unsigned int)(kAtomMax - kAtomMin); ++i)
    {
        bool taken = false;

        for (container::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
        {
            if (it->second.atom == candidate)
            {
                taken = true;
                break;
            }
        }

        if (!taken)
        {
            return candidate;
        }

        if (candidate == kAtomMax)
        {
            candidate = kAtomMin;
        }
        else
        {
            ++candidate;
        }
    }

    return 0;
}

ATOM ClassRegistrator::Append(const WNDCLASSA* wndClass)
{
    if (!wndClass || !wndClass->lpszClassName || !wndClass->lpfnWndProc)
    {
        return 0;
    }

    const std::string name(wndClass->lpszClassName);

    if (_classes.find(name) != _classes.end())
    {
        return 0;
    }

    ATOM atom = AllocateAtom();

    if (atom == 0)
    {
        return 0;
    }

    WindowClassA window;
    window.style = wndClass->style;
    window.cbClsExtra = wndClass->cbClsExtra;
    window.cbWndExtra = wndClass->cbWndExtra;
    window.hbrBackground = wndClass->hbrBackground;
    window.hCursor = wndClass->hCursor;
    window.hIcon = wndClass->hIcon;
    window.hInstance = wndClass->hInstance;
    window.lpfnWndProc = wndClass->lpfnWndProc;
    window.lpszClassName = name;
    window.lpszMenuName = wndClass->lpszMenuName ? wndClass->lpszMenuName : "";
    window.atom = atom;

    _classes.insert(std::make_pair(name, window));

    _nextAtom = (atom == kAtomMax) ? kAtomMin : (ATOM)(atom + 1);

    return atom;
}

bool ClassRegistrator::Find(const std::string& name, WindowClassA& window) const
{
    container::const_iterator i = _classes.find(name);

    if (i != _classes.end())
    {
        window = i->second;
        return true;
    }

    return false;
}

const ClassRegistrator::container& ClassRegistrator::GetClasses() const
{
    return _classes;
}
