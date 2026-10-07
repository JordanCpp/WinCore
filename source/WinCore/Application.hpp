// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Application_hpp
#define WinCore_Application_hpp

#include <WinCore/ClassRegistrator.hpp>
#include <WinCore/WindowManager.hpp>
#include <WinCore/WindowCreator.hpp>
#include <WinCore/Initializer.hpp>
#include <WinCore/EventHandler.hpp>
#include <WinCore/SharedCreator.hpp>
#include <WinCore/Ticks.hpp>
#include <WinCore/Cursor.hpp>
#include <WinCore/AsyncKey.hpp>
#include <WinCore/OpenGLFunc.hpp>

class Application
{
public:
	Application();
	~Application();
	Initializer      _initializer;
	WindowManager    _windowManager;
	EventHandler     _eventHandler;
	ClassRegistrator _classRegistrator;
	WindowCreator    _windowCreator;
	SharedCreator    _sharedCreator;
	Ticks            _ticks;
	Cursor           _cursor;
	AsyncKey         _asyncKey;
	OpenGLFuncs      _openGLFuncs;
};

Application& MainApplication();

#endif
