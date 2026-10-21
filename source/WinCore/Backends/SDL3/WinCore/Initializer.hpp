// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_Initializer_hpp
#define WinCore_SDL3_Initializer_hpp

class Initializer
{
public:
    Initializer();
    ~Initializer();

    bool IsInitialized() const;

private:
    Initializer(const Initializer&);
    Initializer& operator=(const Initializer&);

    bool _initialized;
};

#endif
