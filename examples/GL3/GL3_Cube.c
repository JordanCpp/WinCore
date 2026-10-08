/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#define OPENGL_IMPLEMENTATION
#include "OpenGL.h" 

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <WinCore/Windows.h>

typedef HGLRC(*WINAPI PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

float cubeVertices[] = 
{
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f
};

unsigned int cubeIndices[] = 
{
    0, 1, 2,  2, 3, 0,
    4, 5, 6,  6, 7, 4,
    3, 2, 6,  6, 7, 3,
    0, 1, 5,  5, 4, 0,
    1, 5, 6,  6, 2, 1,
    0, 4, 7,  7, 3, 0
};

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"void main() {\n"
"   gl_Position = model * vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\n\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

unsigned int shaderProgram, VAO, VBO, EBO;
unsigned int modelLoc;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
    {
        int width = (int)LOWORD(lParam);
        int height = (int)HIWORD(lParam);
        if (height == 0) height = 1;
        glViewport(0, 0, width, height);
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void InitShaders()
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    modelLoc = glGetUniformLocation(shaderProgram, "model");
}

int main()
{
    WNDCLASS wc;
    MSG      msg;
    HWND     hwnd;
    HDC      hDC;
    HGLRC    hRC;
    int      pixelFormat;
    float    angle = 0.0f;
    BOOL     running;
    PIXELFORMATDESCRIPTOR pfd;

    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreGL3Demo";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);

    /*
    wc.style = CS_OWNDC;
    */

    if (!RegisterClass(&wc)) return 1;

    hwnd = CreateWindow(
        wc.lpszClassName, "WinCore - OpenGL 3.3 Core Profile Cube",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, wc.hInstance, NULL);

    if (!hwnd) return 1;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    hDC = GetDC(hwnd);

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB)
    {
        fprintf(stderr, "Failed to load wglCreateContextAttribsARB\n");
        return 1;
    }

    int attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    hRC = wglCreateContextAttribsARB(hDC, NULL, attribList);
    if (!hRC) return 1;

    wglMakeCurrent(hDC, hRC);

    OpenGL_Compatibility_Init(3, 3);

    InitShaders();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    msg.message = WM_NULL;
    running = TRUE;

    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = FALSE;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!running) break;

        glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float c = cosf(angle);
        float s = sinf(angle);
        float modelMatrix[16] = {
            c,   0.0f,  s,    0.0f,
            s * s, c,    -s * c,  0.0f,
           -s * c, s,     c * c,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        angle += 0.01f;

        SwapBuffers(hDC);

        Sleep(16);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);

    return 0;
}
