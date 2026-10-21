// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <new>
#include <WinCore/SharedCreator.hpp>

SharedCreator::SharedCreator()
{
}

Shared* SharedCreator::Create(const char* lpLibFileName)
{
    if (!lpLibFileName || !*lpLibFileName)
    {
        return NULL;
    }

    Shared* shared = new (std::nothrow) Shared;

    if (!shared)
    {
        return NULL;
    }

    if (!shared->Load(lpLibFileName))
    {
        delete shared;
        return NULL;
    }

    return shared;
}
