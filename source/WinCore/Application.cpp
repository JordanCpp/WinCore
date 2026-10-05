// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>

static Application _application;

Application::Application()
{
}

Application::~Application()
{
}

ATOM Application::RegisterClassImpl(const WNDCLASSA* wndClass)
{
	if (!wndClass) return 0;

	_classRegistrator.Append(wndClass);
	return true;
}

HWND Application::CreateWindowExAImpl(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	Window* window = _windowCreator.Create(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (!window) return NULL;

	HWND hFakeWnd = (HWND)window;

	_windowManager.Append(hFakeWnd, window);

	WindowClassA windowClass;

	if (_classRegistrator.Find(lpClassName, windowClass))
	{
		CREATESTRUCTA cs;
		cs.lpCreateParams = lpParam;
		cs.hInstance      = hInstance;
		cs.hMenu          = hMenu;
		cs.hwndParent     = hWndParent;
		cs.cy             = nHeight;
		cs.cx             = nWidth;
		cs.y              = Y;
		cs.x              = X;
		cs.style          = dwStyle;
		cs.lpszName       = lpWindowName;
		cs.lpszClass      = lpClassName;
		cs.dwExStyle      = dwExStyle;

		windowClass.lpfnWndProc(hFakeWnd, WM_CREATE, 0, (LPARAM)&cs);
	}

	return hFakeWnd;
}

BOOL Application::DestroyWindowImpl(HWND hWnd)
{
	if (!hWnd)
	{
		return false;
	}

	Window* window = _windowManager.Find(hWnd);
	if (!window)
	{
		return false;
	}

	WindowClassA windowClass;
	if (_classRegistrator.Find(window->GetClassName(), windowClass))
	{
		windowClass.lpfnWndProc(hWnd, WM_DESTROY, 0, 0);
		windowClass.lpfnWndProc(hWnd, WM_NCDESTROY, 0, 0);
	}

	return _windowManager.Destroy(hWnd);
}

BOOL Application::GetMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	if (!lpMsg)
	{
		return false;
	}

	MSG msg = { 0 };

	if (_eventHandler.IsRunning())
	{
		if (_eventHandler.WaitEvent(msg))
		{
			lpMsg->hwnd    = msg.hwnd;
			lpMsg->message = msg.message;
			lpMsg->wParam  = msg.wParam;
			lpMsg->lParam  = msg.lParam;
			lpMsg->time    = msg.time;
			lpMsg->pt      = msg.pt;

			if (msg.message == WM_QUIT)
			{
				_eventHandler.StopEvents();

				return false;
			}

			return true;
		}
	}

	return false;
}

BOOL Application::PeekMessageAImpl(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	if (!lpMsg) return false;

	MSG msg = { 0 };
	bool bRemove = (wRemoveMsg == PM_REMOVE);

	if (_eventHandler.GetEvent(msg, bRemove))
	{
		lpMsg->hwnd = msg.hwnd;
		lpMsg->message = msg.message;
		lpMsg->wParam = msg.wParam;
		lpMsg->lParam = msg.lParam;
		lpMsg->time = msg.time;
		lpMsg->pt = msg.pt;

		if (msg.message == WM_QUIT && bRemove)
		{
			_eventHandler.StopEvents();
		}

		return true;
	}

	return false;
}

void Application::PostQuitMessageImpl(int nExitCode)
{
	MSG quitMsg     = { 0 };
	quitMsg.message = WM_QUIT;
	quitMsg.wParam  = (WPARAM)nExitCode;

	_eventHandler.PushMessage(quitMsg);
}

LRESULT Application::DefWindowProcAImpl(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_CLOSE)
	{
		_eventHandler.StopEvents();
	}

	return 0;
}

LRESULT Application::DispatchMessageAImpl(const MSG* lpMsg)
{
	if (!lpMsg)
	{

		return 0;
	}

	if (lpMsg->hwnd)
	{
		Window* window = _windowManager.Find(lpMsg->hwnd);

		if (window)
		{
			WindowClassA windowClass;

			if (_classRegistrator.Find(window->GetClassName(), windowClass))
			{
				return windowClass.lpfnWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
			}
		}
	}
	else
	{
		for (ClassRegistrator::container::const_iterator i = _classRegistrator.GetClasses().begin(); i != _classRegistrator.GetClasses().end(); i++)
		{
			i->second.lpfnWndProc(NULL, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
		}
	}

	return true;
}

BOOL Application::TranslateMessageImpl(const MSG* lpMsg)
{
	return true;
}

HDC Application::GetDCImpl(HWND hWnd)
{
	return (HDC)hWnd;
}

HGLRC Application::wglCreateContextImpl(HDC hdc)
{
	Window* window = _windowManager.Find((HWND)hdc);

	if (window)
	{
		window->CreateContext();
	}

	return (HGLRC)hdc;
}

BOOL Application::wglMakeCurrentImpl(HDC hdc, HGLRC hglrc)
{
	Window* window = _windowManager.Find((HWND)hdc);

	if (window)
	{
		return window->MakeCurrent();
	}

	return false;
}

BOOL Application::SwapBuffers(HDC hdc)
{
	Window* window = _windowManager.Find((HWND)hdc);

	if (window)
	{
		return window->SwapBuffers();
	}

	return false;
}

int Application::ChoosePixelFormatImpl(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd)
{
	if (!hdc || !ppfd)
	{
		return 0;
	}

	return 1;
}

BOOL Application::SetPixelFormatImpl(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd)
{
	if (!hdc || format <= 0 || !ppfd) return false;

	Window* window = _windowManager.Find((HWND)hdc);

	if (window)
	{
		return true;
	}

	return false;
}

HMODULE Application::LoadLibraryAImpl(LPCSTR lpLibFileName)
{
	return (HMODULE)_sharedCreator.Create(lpLibFileName);
}

BOOL Application::FreeLibraryImpl(HMODULE hLibModule)
{
	Shared* shared = (Shared*)(hLibModule);

	shared->Unload();

	return true;
}

FARPROC Application::GetProcAddressImpl(HMODULE hModule, LPCSTR lpProcName)
{
	Shared* shared = (Shared*)(hModule);

	return (FARPROC)shared->GetFunction(lpProcName);
}

int Application::SetDIBitsToDeviceImpl(HDC hdc, int xDest, int yDest, DWORD wDest, DWORD hDest, int xSrc, int ySrc, UINT uStartScan, UINT cScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT colorUse)
{
	if (!hdc || !lpvBits || !lpbmi)
	{
		return 0;
	}

	HWND hWnd = (HWND)hdc;
	Window* window = _windowManager.Find(hWnd);
	if (!window)
	{
		return 0;
	}

	int srcWidth  = lpbmi->bmiHeader.biWidth;
	int biHeight  = lpbmi->bmiHeader.biHeight;
	int srcHeight = (biHeight < 0) ? -biHeight : biHeight;

	BOOL success = window->BlitDIBits(xDest, yDest, static_cast<int>(wDest), static_cast<int>(hDest), xSrc, ySrc, srcWidth, srcHeight, lpvBits, srcWidth, srcHeight, biHeight);

	return success ? cScanLines : 0;
}

int Application::StretchDIBitsImpl(HDC hdc, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop)
{
	if (!hdc || !lpBits || !lpbmi)
	{
		return 0;
	}

	HWND hWnd = (HWND)hdc;
	Window* window = _windowManager.Find(hWnd);
	if (!window)
	{
		return 0;
	}

	int srcWidth  = lpbmi->bmiHeader.biWidth;
	int biHeight  = lpbmi->bmiHeader.biHeight;
	int srcHeight = std::abs(biHeight);

	BOOL success = window->BlitDIBits(xDest, yDest, wDest, hDest, xSrc, ySrc, wSrc, hSrc, lpBits, srcWidth, srcHeight, biHeight);

	return success ? hSrc : 0;
}

int Application::ReleaseDCImpl(HWND hWnd, HDC hDC)
{
	if (!hDC)
	{
		return 0;
	}

	Window* window = _windowManager.Find(hWnd ? hWnd : (HWND)hDC);
	if (!window)
	{
		return 0;
	}

	return 1;
}

BOOL Application::GetClientRectAImpl(HWND hWnd, LPRECT lpRect)
{
	if (!hWnd || !lpRect)
	{
		return false;
	}

	Window* window = _windowManager.Find(hWnd);
	if (!window)
	{
		return false;
	}

	return window->GetClientRectImpl(lpRect);
}

BOOL Application::ShowWindowAImpl(HWND hWnd, int nCmdShow)
{
	if (!hWnd) return false;

	Window* window = _windowManager.Find(hWnd);
	if (!window) return false;

	return window->ShowWindow(nCmdShow);
}

BOOL Application::UpdateWindowImpl(HWND hWnd)
{
	if (!hWnd) return false;

	Window* window = _windowManager.Find(hWnd);
	if (!window) return false;

	return window->UpdateWindow();
}

Application& MainApplication()
{
	return _application;
}
