// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_Keys_h
#define WinCore_Keys_h

// --- Mouse Constants (Virtual Keys for Mouse) ---
#define VK_LBUTTON        0x01 // Left mouse button
#define VK_RBUTTON        0x02 // Right mouse button
#define VK_CANCEL         0x03 // Control-break processing
#define VK_MBUTTON        0x04 // Middle mouse button (scroll wheel)
#define VK_XBUTTON1       0x05 // X1 mouse button
#define VK_XBUTTON2       0x06 // X2 mouse button

// --- System Control Keys ---
#define VK_BACK           0x08 // BACKSPACE key
#define VK_TAB            0x09 // TAB key
#define VK_CLEAR          0x0C // CLEAR key (Num 5 with NumLock off)
#define VK_RETURN         0x0D // ENTER key
#define VK_SHIFT          0x10 // SHIFT key (generic)
#define VK_CONTROL        0x11 // CTRL key (generic)
#define VK_MENU           0x12 // ALT key (generic)
#define VK_PAUSE          0x13 // PAUSE key
#define VK_CAPITAL        0x14 // CAPS LOCK key
#define VK_ESCAPE         0x1B // ESC key
#define VK_SPACE          0x20 // SPACEBAR key

// --- Navigation and Display Modes ---
#define VK_PRIOR          0x21 // PAGE UP key
#define VK_NEXT           0x22 // PAGE DOWN key
#define VK_END            0x23 // END key
#define VK_HOME           0x24 // HOME key
#define VK_LEFT           0x25 // LEFT ARROW key
#define VK_UP             0x26 // UP ARROW key
#define VK_RIGHT          0x27 // RIGHT ARROW key
#define VK_DOWN           0x28 // DOWN ARROW key
#define VK_SELECT         0x29 // SELECT key
#define VK_PRINT          0x2A // PRINT key
#define VK_EXECUTE        0x2B // EXECUTE key
#define VK_SNAPSHOT       0x2C // PRINT SCREEN key
#define VK_INSERT         0x2D // INS key
#define VK_DELETE         0x2E // DEL key
#define VK_HELP           0x2F // HELP key

// --- Alphanumeric Block (0-9 and A-Z) ---
// In WinAPI, these correspond directly to their uppercase ASCII values:
#define VK_0              0x30
#define VK_1              0x31
#define VK_2              0x32
#define VK_3              0x33
#define VK_4              0x34
#define VK_5              0x35
#define VK_6              0x36
#define VK_7              0x37
#define VK_8              0x38
#define VK_9              0x39

#define VK_A              0x41
#define VK_B              0x42
#define VK_C              0x43
#define VK_D              0x44
#define VK_E              0x45
#define VK_F              0x46
#define VK_G              0x47
#define VK_H              0x48
#define VK_I              0x49
#define VK_J              0x4A
#define VK_K              0x4B
#define VK_L              0x4C
#define VK_M              0x4D
#define VK_N              0x4E
#define VK_O              0x4F
#define VK_P              0x50
#define VK_Q              0x51
#define VK_R              0x52
#define VK_S              0x53
#define VK_T              0x54
#define VK_U              0x55
#define VK_V              0x56
#define VK_W              0x57
#define VK_X              0x58
#define VK_Y              0x59
#define VK_Z              0x5A

// --- OS System Keys ---
#define VK_LWIN           0x5B // Left Windows key
#define VK_RWIN           0x5C // Right Windows key
#define VK_APPS           0x5D // Applications / Context Menu key
#define VK_SLEEP          0x5F // Computer Sleep key

// --- Numeric Keypad (Numpad) ---
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A // Multiply key (*)
#define VK_ADD            0x6B // Add key (+)
#define VK_SEPARATOR      0x6C // Separator key
#define VK_SUBTRACT       0x6D // Subtract key (-)
#define VK_DECIMAL        0x6E // Decimal key (.)
#define VK_DIVIDE         0x6F // Divide key (/)

// --- Functional Keys ---
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

// --- Lock States ---
#define VK_NUMLOCK        0x90 // NUM LOCK key
#define VK_SCROLL         0x91 // SCROLL LOCK key

// --- Distinguishable Modifier Keys (Left / Right) ---
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4 // Left ALT key
#define VK_RMENU          0xA5 // Right ALT key

// --- OEM Specific Keys (Punctuation) ---
#define VK_OEM_1          0xBA // Used for ';:' key on US standard keyboard
#define VK_OEM_PLUS       0xBB // For any country/region, the '+' key
#define VK_OEM_COMMA      0xBC // For any country/region, the ',' key
#define VK_OEM_MINUS      0xBD // For any country/region, the '-' key
#define VK_OEM_PERIOD     0xBE // For any country/region, the '.' key
#define VK_OEM_2          0xBF // Used for '/?' key on US standard keyboard
#define VK_OEM_3          0xC0 // Used for '`~' key on US standard keyboard (Tilde)
#define VK_OEM_4          0xDB // Used for '[{' key on US standard keyboard
#define VK_OEM_5          0xDC // Used for '\|' key on US standard keyboard
#define VK_OEM_6          0xDD // Used for ']}' key on US standard keyboard
#define VK_OEM_7          0xDE // Used for '"'' key on US standard keyboard

#endif
