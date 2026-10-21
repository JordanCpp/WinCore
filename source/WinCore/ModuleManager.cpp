// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/ModuleManager.hpp>
#include <WinCore/SharedCreator.hpp>

ModuleManager::ModuleManager(SharedCreator& sharedCreator) :
    _nextHandle(0x10000),
    _sharedCreator(sharedCreator)
{
}

ModuleManager::~ModuleManager()
{
    for (container::iterator i = _modules.begin(); i != _modules.end(); ++i)
    {
        delete i->second;
    }

    _modules.clear();
}

HMODULE ModuleManager::Create(LPCSTR lpLibFileName)
{
    Shared* shared = _sharedCreator.Create(lpLibFileName);

    if (!shared)
    {
        return NULL;
    }

    HMODULE handle = (HMODULE)_nextHandle++;
    _modules.insert(std::make_pair(handle, shared));

    return handle;
}

Shared* ModuleManager::Find(HMODULE hModule)
{
    container::iterator i = _modules.find(hModule);

    if (i != _modules.end())
    {
        return i->second;
    }

    return NULL;
}

BOOL ModuleManager::Destroy(HMODULE hModule)
{
    container::iterator i = _modules.find(hModule);

    if (i == _modules.end())
    {
        return FALSE;
    }

    Shared* shared = i->second;
    _modules.erase(i);
    delete shared;

    return TRUE;
}
