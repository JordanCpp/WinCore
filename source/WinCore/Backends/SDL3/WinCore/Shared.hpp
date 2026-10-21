// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Backends_SDL3_Shared_hpp
#define WinCore_Backends_SDL3_Shared_hpp

#include <SDL3/SDL_loadso.h>

class Shared
{
public:
    Shared();
    ~Shared();

    bool  Load(const char* path);
    void  Unload();
    void* GetFunction(const char* name);
    bool  IsLoaded() const;

private:
    Shared(const Shared&);
    Shared& operator=(const Shared&);

    SDL_SharedObject* _object;
};

#endif
