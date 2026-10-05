// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Application_hpp
#define WinCore_Application_hpp

#include <WinCore/ClassRegistrator.hpp>
#include <WinCore/WindowManager.hpp>
#include <WinCore/WindowCreator.hpp>
#include <WinCore/Initializer.hpp>
#include <WinCore/EventHandler.hpp>
#include <WinCore/SharedCreator.hpp>

class Application
{
public:
	Application();
	~Application();
	ATOM RegisterClassImpl(const WNDCLASSA* wndClass);
	HWND CreateWindowExAImpl(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	BOOL DestroyWindowImpl(HWND hWnd);
	BOOL GetMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
	BOOL PeekMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
	void PostQuitMessageImpl(int nExitCode);
	LRESULT DefWindowProcAImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT DispatchMessageAImpl(const MSG* lpMsg);
	BOOL TranslateMessageImpl(const MSG* lpMsg);
	HDC GetDCImpl(HWND hWnd);
	HGLRC wglCreateContextImpl(HDC hdc);
	BOOL wglMakeCurrentImpl(HDC hdc, HGLRC hglrc);
	BOOL SwapBuffers(HDC hdc);
	int ChoosePixelFormatImpl(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd);
	BOOL SetPixelFormatImpl(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd);
	HMODULE LoadLibraryAImpl(LPCSTR lpLibFileName);
	BOOL FreeLibraryImpl(HMODULE hLibModule);
	FARPROC GetProcAddressImpl(HMODULE hModule, LPCSTR  lpProcName);
private:
	Initializer      _initializer;
	EventHandler     _eventHandler;
	ClassRegistrator _classRegistrator;
	WindowCreator    _windowCreator;
	WindowManager    _windowManager;
	SharedCreator    _sharedCreator;
};

Application& MainApplication();

#endif
