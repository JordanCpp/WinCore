# WinCore

A ultra-portable, cross-platform windowing and graphics abstraction framework strictly written in compliance with the **ISO C++98** standard.

**WinCore** features a unique low-level architectural design: instead of just wrapping APIs, it provides a high-fidelity **hardware/software emulation layer for the Win32 sub-system**. This allows legacy, procedural C/C++ Windows applications (utilizing a standard window procedure callback and a `GetMessage`/`DispatchMessage` game loop) to be compiled natively on modern non-Windows platforms (such as Linux or macOS) without a single modification to the core codebase.

Under the hood, WinCore intercepts classical Win32 entry-points and routes them through cross-platform rendering backends like **SDL3**, **SDL2**, or a headless **NULL** system.

---

## Project Status & Roadmap

⚠️ **Project Status:** This framework is currently under active development. Core Win32 emulation and OpenGL context switching are functional, but specific API entry-points are still being implemented.

---

### Future Subsystem Backends:
- **SDL1 Backend:** Planned for deployment to target vintage legacy hardware and highly resource-constrained environments.
- **XLib Backend:** Planned for native Linux/Unix environments to provide lightweight execution paths entirely independent of third-party media libraries.

---

## Core Concept

The core philosophy of **WinCore** is to establish the classic, deterministic Win32 API as a universal, cross-platform bridge between diverse computing devices and operating systems. 

Instead of forcing developers to refit or completely rewrite time-tested procedural C/C++ rendering logic for modern abstract APIs, WinCore transforms legacy Windows patterns into a standard middleware. This allows historically platform-locked codebases to migrate seamlessly, running with high fidelity across entirely different software topologies and hardware form factors.

---

## Scope & Architectural Focus

To ensure lightweight execution, ultra-portability, and clean maintenance, **WinCore does not aim to emulate the entire gargantuan Win32 API surface**. Instead, it strictly targets the essential multimedia and game-loop sub-systems required to run high-performance 2D and 3D engines.

---

### Supported & Planned Sub-systems:
- **Core Abstraction Layer (Implemented):** Window lifecycles (`HWND`), system events, message loops (`MSG`), and precise device input polling (mouse/keyboard).
- **3D Graphics Layer (Implemented):** Modern and legacy **OpenGL** device context integration (`HDC`, `HGLRC`, `WGL`).
- **Modern Compute Layer (Roadmap):** Native **Vulkan** surface initialization bridge.
- **Legacy 2D Graphics Layer (Roadmap):** **GDI** software pixel pipelines and **DirectDraw** surfaces for classic sprite-based execution paths.
- **Audio Sub-system (Roadmap):** Low-level **DirectSound** buffer management and hardware mixer emulation.

This tailored subset covers the execution matrix for the vast majority of historical and modern interactive software, keeping the framework lean, deterministic, and highly optimized.

---

## Core Execution Model

Instead of including native Microsoft headers, you link against WinCore. Legacy procedural entry-points compile and process identically across platforms:

```c
// This exact Win32/WGL procedural logic compiles natively on Linux using WinCore!
#include <stdio.h>
#include <WinCore/GL/GL.h>
#include <WinCore/Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            printf("Window initialized successfully!\n");
            break;
        case WM_DESTROY:
            PostQuitMessage(0); // Safely appends WM_QUIT to internal loop queue
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "WinCoreDemoEngine";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "WinCore Engine Framework", 
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                             800, 600, NULL, NULL, NULL, NULL);

    HDC hDC = GetDC(hwnd);
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    MSG msg;
    // GetMessage yields true until WM_QUIT is successfully read and cleared
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg); // Target-directed routing via WindowManager

        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SwapBuffers(hDC);
    }
    return 0;
}
```

---

## Compilation Guidelines

The architecture uses Modern CMake targeting a strict C++98 translation environment.

### Setting standard backend compilation targets:
```bash
mkdir build && cd build
cmake -DBACKEND_SDL2=ON ..   # Configures compilation for SDL2 target
cmake --build .
```

On Windows builds, CMake triggers a `POST_BUILD` asset deployment step, automatically validating architecture width (x86/x64) and provisioning the binary execution path with corresponding runtime components (`SDL2.dll` or `SDL3.dll`).

---

## Licensing

Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp).  
Licensed under the terms of the **LGPL-3.0-or-later** license. Independent software vendors can link against this library without open-sourcing their application code, provided the framework modifications remain open-source.

---

## Showcase Gallery

The images below demonstrate WinCore's hardware/software subsystem emulation layer natively executing classical OpenGL 1.2 fixed-function pipeline techniques across platforms:

---

### Core Fixed-Function Techniques

| Gouraud vs Flat Shading | Dynamic Spotlight |
| :---: | :---: |
| <img src="screenshots/OpenGL1.2GouraudvsFlat.png" width="400" alt="Gouraud vs Flat Shading"/> | <img src="screenshots/OpenGL1.2DynamicSpotlight.png" width="400" alt="Dynamic Spotlight"/> |

| Textured 3D Cube | Vertex Arrays |
| :---: | :---: |
| <img src="screenshots/OpenGL1.2Textured3DCube.png" width="400" alt="Textured 3D Cube"/> | <img src="screenshots/OpenGL1.2VertexArrays.png" width="400" alt="Vertex Arrays"/> |
