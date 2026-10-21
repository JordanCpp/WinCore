// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_ModuleManager_hpp
#define WinCore_ModuleManager_hpp

#include <map>
#include <WinCore/Kernel32.h>
#include <WinCore/Shared.hpp>

class SharedCreator;

class ModuleManager
{
public:
    ModuleManager(SharedCreator& sharedCreator);
    ~ModuleManager();

    HMODULE Create(LPCSTR lpLibFileName);
    Shared* Find(HMODULE hModule);
    BOOL    Destroy(HMODULE hModule);

private:
    ModuleManager(const ModuleManager&);
    ModuleManager& operator=(const ModuleManager&);

    typedef std::map<HMODULE, Shared*> container;
    container _modules;
    size_t    _nextHandle;

    SharedCreator& _sharedCreator;
};

#endif
