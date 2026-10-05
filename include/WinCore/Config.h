// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Config_h
#define WinCore_Config_h

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef WINCORE_BUILD_DLL
        #ifdef __GNUC__
            #define WINCORE_API __attribute__((dllexport))
        #else
            #define WINCORE_API __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define WINCORE_API __attribute__((dllimport))
        #else
            #define WINCORE_API __declspec(dllimport)
        #endif
    #endif
#else
    #if __GNUC__ >= 4
        #define WINCORE_API __attribute__((visibility("default")))
    #else
        #define WINCORE_API
    #endif
#endif

#endif
