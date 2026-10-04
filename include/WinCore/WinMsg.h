// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_WinMsg_h
#define WinCore_WinMsg_h

// --- Window Lifecycle Messages ---
#define WM_NULL           0x0000
#define WM_CREATE         0x0001 // Sent when a window is being created
#define WM_DESTROY        0x0002 // Sent when a window is being destroyed
#define WM_MOVE           0x0003 // Sent when a window has been moved
#define WM_SIZE           0x0005 // Sent when a window has changed size
#define WM_ACTIVATE       0x0006 // Sent when a window is activated or deactivated
#define WM_SETFOCUS       0x0007 // Sent after gaining keyboard focus
#define WM_KILLFOCUS      0x0008 // Sent before losing keyboard focus
#define WM_ENABLE         0x000A // Sent when an application changes the enabled state
#define WM_PAINT          0x000F // Sent when the system requests a window redraw
#define WM_CLOSE          0x0010 // Sent as a signal that a window should terminate
#define WM_QUERYENDSESSION 0x0011 // Sent when the user chooses to end the session
#define WM_QUIT           0x0012 // Sent to terminate a thread's message loop
#define WM_QUERYOPEN      0x0013 // Sent when the user requests an iconified window to restore
#define WM_ERASEBKGND     0x0014 // Sent when the window background must be erased
#define WM_SHOWWINDOW     0x0018 // Sent when a window is about to be hidden or shown

// --- Non-Client Area Messages ---
#define WM_NCCREATE       0x0081 // Sent prior to the WM_CREATE message
#define WM_NCDESTROY      0x0082 // Sent when the non-client area is being destroyed
#define WM_NCCALCSIZE     0x0083 // Sent when the size and position of the client area need to be calculated
#define WM_NCHITTEST      0x0084 // Sent to determine what part of the window corresponds to a point
#define WM_NCPAINT        0x0085 // Sent when the frame of the window must be painted
#define WM_NCACTIVATE     0x0086 // Sent when the non-client area needs to change its active state

// --- Keyboard Input ---
#define WM_KEYDOWN        0x0100 // Sent when a nonsystem key is pressed
#define WM_KEYUP          0x0101 // Sent when a nonsystem key is released
#define WM_CHAR           0x0102 // Sent when a keystroke is translated into a character
#define WM_DEADCHAR       0x0103 // Sent when a dead key is translated into a character
#define WM_SYSKEYDOWN     0x0104 // Sent when the user presses Alt + key
#define WM_SYSKEYUP       0x0105 // Sent when the user releases Alt + key

// --- Mouse Input Messages  ---
#define WM_MOUSEMOVE      0x0200 // Sent when the cursor moves
#define WM_LBUTTONDOWN    0x0201 // Sent when the left mouse button is pressed
#define WM_LBUTTONUP      0x0202 // Sent when the left mouse button is released
#define WM_LBUTTONDBLCLK  0x0203 // Sent when the left mouse button is double-clicked
#define WM_RBUTTONDOWN    0x0204 // Sent when the right mouse button is pressed
#define WM_RBUTTONUP      0x0205 // Sent when the right mouse button is released
#define WM_RBUTTONDBLCLK  0x0206 // Sent when the right mouse button is double-clicked
#define WM_MBUTTONDOWN    0x0207 // Sent when the middle mouse button is pressed
#define WM_MBUTTONUP      0x0208 // Sent when the middle mouse button is released
#define WM_MBUTTONDBLCLK  0x0209 // Sent when the middle mouse button is double-clicked
#define WM_MOUSEWHEEL     0x020A // Sent when the mouse wheel is rotated

// --- System Messages ---
#define WM_SYSCOMMAND     0x0112 // Sent when the user selects a command from the Window menu
#define WM_TIMER          0x0113 // Sent when a timer expires
#define WM_ENTERSIZEMOVE  0x0231 // Sent when a window enters the moving or sizing modal loop
#define WM_EXITSIZEMOVE   0x0232 // Sent when a window exits the moving or sizing modal loop

// --- User-Defined Messages ---
#define WM_USER           0x0400 // Used to define private messages for custom controls

#endif
