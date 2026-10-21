// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_ClassRegistrator_hpp
#define WinCore_ClassRegistrator_hpp

#include <map>
#include <WinCore/WindowClassA.hpp>

class ClassRegistrator
{
public:
    typedef std::map<std::string, WindowClassA> container;
    ClassRegistrator();
    ATOM Append(const WNDCLASSA* wndClass);
    bool Find(const std::string& name, WindowClassA& window) const;
    const container& GetClasses() const;
private:
    ClassRegistrator(const ClassRegistrator&);
    ClassRegistrator& operator=(const ClassRegistrator&);
    ATOM AllocateAtom() const;
    container _classes;
    ATOM      _nextAtom;
};

#endif
