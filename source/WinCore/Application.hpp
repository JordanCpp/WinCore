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
	int SetDIBitsToDeviceImpl(HDC hdc, int xDest, int yDest, DWORD wDest, DWORD hDest, int xSrc, int ySrc, UINT uStartScan, UINT cScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT colorUse);
	int StretchDIBitsImpl(HDC hdc, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop);
	int ReleaseDCImpl(HWND hWnd, HDC hDC);
	BOOL GetClientRectAImpl(HWND hWnd, LPRECT lpRect);
	BOOL ShowWindowAImpl(HWND hWnd, int nCmdShow);
	BOOL UpdateWindowImpl(HWND hWnd);
//private:
	Initializer      _initializer;
	EventHandler     _eventHandler;
	ClassRegistrator _classRegistrator;
	WindowCreator    _windowCreator;
	WindowManager    _windowManager;
	SharedCreator    _sharedCreator;
	Ticks            _ticks;
	Cursor           _cursor;
};

Application& MainApplication();

#endif
