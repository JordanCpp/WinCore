/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#define OPENGL_IMPLEMENTATION
#include "OpenGL.h" 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <WinCore/Windows.h>

typedef HGLRC(*WINAPI PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

static const char* GlErrorString(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:                      return "GL_NO_ERROR";
    case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:                               return "UNKNOWN_GL_ERROR";
    }
}

static int gl_check_error(const char* where)
{
    GLenum err;
    int ok = 1;

    while ((err = glGetError()) != GL_NO_ERROR)
    {
        fprintf(stderr, "[GL ERROR] %s -> %s (0x%04X)\n", where, GlErrorString(err), (unsigned)err);
        ok = 0;
    }

    return ok;
}

static int check_shader(GLuint shader, const char* label)
{
    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            char* log = (char*)malloc((size_t)len);
            if (log)
            {
                GLsizei written = 0;
                glGetShaderInfoLog(shader, len, &written, log);
                fprintf(stderr, "[SHADER ERROR] %s:\n%s\n", label, log);
                free(log);
            }
        }
        return 0;
    }

    return 1;
}

static int check_program(GLuint program, const char* label)
{
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (!status)
    {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            char* log = (char*)malloc((size_t)len);
            if (log)
            {
                GLsizei written = 0;
                glGetProgramInfoLog(program, len, &written, log);
                fprintf(stderr, "[PROGRAM ERROR] %s:\n%s\n", label, log);
                free(log);
            }
        }
        return 0;
    }

    return 1;
}

static void mat4_perspective(float* m, float fovyRad, float aspect, float znear, float zfar)
{
    float f = 1.0f / tanf(fovyRad * 0.5f);

    memset(m, 0, 16 * sizeof(float));

    m[0] = f / aspect;
    m[5] = f;
    m[10] = (zfar + znear) / (znear - zfar);
    m[11] = -1.0f;
    m[14] = (2.0f * zfar * znear) / (znear - zfar);
}

static void mat4_lookAt(float* m,
    float ex, float ey, float ez,
    float cx, float cy, float cz,
    float ux, float uy, float uz)
{
    float fx = cx - ex, fy = cy - ey, fz = cz - ez;
    float fl = sqrtf(fx * fx + fy * fy + fz * fz);
    fx /= fl; fy /= fl; fz /= fl;

    float sx = fy * uz - fz * uy;
    float sy = fz * ux - fx * uz;
    float sz = fx * uy - fy * ux;
    float sl = sqrtf(sx * sx + sy * sy + sz * sz);
    sx /= sl; sy /= sl; sz /= sl;

    float ux2 = sy * fz - sz * fy;
    float uy2 = sz * fx - sx * fz;
    float uz2 = sx * fy - sy * fx;

    m[0] = sx; m[4] = sy; m[8] = sz; m[12] = -(sx * ex + sy * ey + sz * ez);
    m[1] = ux2; m[5] = uy2; m[9] = uz2; m[13] = -(ux2 * ex + uy2 * ey + uz2 * ez);
    m[2] = -fx; m[6] = -fy; m[10] = -fz; m[14] = (fx * ex + fy * ey + fz * ez);
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}

static void mat4_rotateY(float* m, float angleRad)
{
    float c = cosf(angleRad);
    float s = sinf(angleRad);

    m[0] = c; m[4] = 0.0f; m[8] = s; m[12] = 0.0f;
    m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f; m[13] = 0.0f;
    m[2] = -s; m[6] = 0.0f; m[10] = c; m[14] = 0.0f;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}

static void mat4_rotateX(float* m, float angleRad)
{
    float c = cosf(angleRad);
    float s = sinf(angleRad);

    m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f; m[12] = 0.0f;
    m[1] = 0.0f; m[5] = c; m[9] = -s; m[13] = 0.0f;
    m[2] = 0.0f; m[6] = s; m[10] = c; m[14] = 0.0f;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}

static void mat4_mul(float* out, const float* a, const float* b)
{
    float tmp[16];
    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            tmp[col * 4 + row] =
                a[0 * 4 + row] * b[col * 4 + 0] +
                a[1 * 4 + row] * b[col * 4 + 1] +
                a[2 * 4 + row] * b[col * 4 + 2] +
                a[3 * 4 + row] * b[col * 4 + 3];
        }
    }
    memcpy(out, tmp, sizeof(tmp));
}

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
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\n\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

unsigned int shaderProgram = 0;
unsigned int VAO = 0;
unsigned int VBO = 0;
unsigned int EBO = 0;
int modelLoc = -1;
int viewLoc = -1;
int projLoc = -1;

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
        gl_check_error("glViewport (WM_SIZE)");
        return 0;
    }
    case WM_CLOSE:
        printf("[WM_CLOSE] hwnd=%p\n", (void*)hwnd);
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        printf("[WM_DESTROY] hwnd=%p\n", (void*)hwnd);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int InitShaders(void)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (!vertexShader) { fprintf(stderr, "[FAIL] glCreateShader(VERTEX) == 0\n"); return 0; }
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    if (!check_shader(vertexShader, "vertex")) return 0;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!fragmentShader) { fprintf(stderr, "[FAIL] glCreateShader(FRAGMENT) == 0\n"); return 0; }
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    if (!check_shader(fragmentShader, "fragment")) return 0;

    shaderProgram = glCreateProgram();
    if (!shaderProgram) { fprintf(stderr, "[FAIL] glCreateProgram() == 0\n"); return 0; }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (!check_program(shaderProgram, "main")) return 0;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projLoc = glGetUniformLocation(shaderProgram, "projection");

    if (modelLoc < 0) fprintf(stderr, "[WARN] uniform 'model' not found\n");
    if (viewLoc < 0) fprintf(stderr, "[WARN] uniform 'view' not found\n");
    if (projLoc < 0) fprintf(stderr, "[WARN] uniform 'projection' not found\n");

    return gl_check_error("InitShaders");
}

int main(void)
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

    printf("[STEP] RegisterClass\n");
    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpszClassName = "WinCoreGL3Demo";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.style = CS_OWNDC;

    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "[FAIL] RegisterClass\n");
        return 1;
    }

    printf("[STEP] CreateWindow\n");
    hwnd = CreateWindow(
        wc.lpszClassName, "WinCore - OpenGL 3.3 Core Profile Cube",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, wc.hInstance, NULL);

    if (!hwnd)
    {
        fprintf(stderr, "[FAIL] CreateWindow == NULL\n");
        return 1;
    }
    printf("[ OK ] hwnd=%p\n", (void*)hwnd);

    printf("[STEP] ShowWindow / UpdateWindow\n");
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    printf("[STEP] GetDC\n");
    hDC = GetDC(hwnd);
    if (!hDC)
    {
        fprintf(stderr, "[FAIL] GetDC == NULL\n");
        return 1;
    }
    printf("[ OK ] hDC=%p\n", (void*)hDC);

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    printf("[STEP] ChoosePixelFormat\n");
    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    if (pixelFormat <= 0)
    {
        fprintf(stderr, "[FAIL] ChoosePixelFormat == %d\n", pixelFormat);
        return 1;
    }
    printf("[ OK ] pixelFormat=%d\n", pixelFormat);

    printf("[STEP] SetPixelFormat\n");
    if (!SetPixelFormat(hDC, pixelFormat, &pfd))
    {
        fprintf(stderr, "[FAIL] SetPixelFormat\n");
        return 1;
    }

    printf("[STEP] wglGetProcAddress(wglCreateContextAttribsARB)\n");
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB)
    {
        fprintf(stderr, "[FAIL] wglCreateContextAttribsARB == NULL\n");
        return 1;
    }

    int attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    printf("[STEP] wglCreateContextAttribsARB(3.3 core)\n");
    hRC = wglCreateContextAttribsARB(hDC, NULL, attribList);
    if (!hRC)
    {
        fprintf(stderr, "[FAIL] wglCreateContextAttribsARB == NULL\n");
        return 1;
    }
    printf("[ OK ] hRC=%p\n", (void*)hRC);

    printf("[STEP] wglMakeCurrent\n");
    if (!wglMakeCurrent(hDC, hRC))
    {
        fprintf(stderr, "[FAIL] wglMakeCurrent\n");
        return 1;
    }

    printf("[STEP] OpenGL_Compatibility_Init(3, 3)\n");
    OpenGL_Compatibility_Init(3, 3);
    if (!gl_check_error("OpenGL_Compatibility_Init"))
    {
        fprintf(stderr, "[FAIL] OpenGL_Compatibility_Init\n");
        return 1;
    }

    {
        RECT rc;
        if (GetClientRect(hwnd, &rc))
        {
            int vw = (int)(rc.right - rc.left);
            int vh = (int)(rc.bottom - rc.top);
            if (vw <= 0) vw = 800;
            if (vh <= 0) vh = 600;
            glViewport(0, 0, vw, vh);
            printf("[ OK ] glViewport(0,0,%d,%d)\n", vw, vh);
        }
        else
        {
            glViewport(0, 0, 800, 600);
            printf("[WARN] GetClientRect failed, using 800x600 viewport\n");
        }
    }

    printf("[STEP] InitShaders\n");
    if (!InitShaders())
    {
        fprintf(stderr, "[FAIL] InitShaders\n");
        return 1;
    }

    printf("[STEP] Create VAO/VBO/EBO\n");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    if (!VAO || !VBO || !EBO)
    {
        fprintf(stderr, "[FAIL] glGen* -> VAO=%u VBO=%u EBO=%u\n", VAO, VBO, EBO);
        return 1;
    }

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

    if (!gl_check_error("setup buffers/attribs"))
    {
        fprintf(stderr, "[FAIL] ошибка при настройке буферов/атрибутов\n");
        return 1;
    }

    msg.message = WM_NULL;
    running = TRUE;

    printf("[STEP] enter main loop\n");

    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                printf("[LOOP] WM_QUIT received\n");
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

        float rotY[16];
        float rotX[16];
        float model[16];
        float view[16];
        float proj[16];

        mat4_rotateY(rotY, angle);
        mat4_rotateX(rotX, angle * 0.5f);
        mat4_mul(model, rotX, rotY);

        mat4_lookAt(view,
            0.0f, 0.0f, 3.0f,   /* eye    */
            0.0f, 0.0f, 0.0f,   /* center */
            0.0f, 1.0f, 0.0f);  /* up     */

        {
            RECT rc;
            float aspect = 800.0f / 600.0f;
            if (GetClientRect(hwnd, &rc))
            {
                int vw = (int)(rc.right - rc.left);
                int vh = (int)(rc.bottom - rc.top);
                if (vw > 0 && vh > 0) aspect = (float)vw / (float)vh;
            }
            mat4_perspective(proj, 0.785398f /* 45° */, aspect, 0.1f, 100.0f);
        }

        if (modelLoc >= 0) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
        if (viewLoc >= 0) glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        if (projLoc >= 0) glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        if (!gl_check_error("draw frame"))
        {
            fprintf(stderr, "[FAIL] ошибка при отрисовке кадра\n");
            running = FALSE;
            break;
        }

        angle += 0.01f;

        if (!SwapBuffers(hDC))
        {
            fprintf(stderr, "[FAIL] SwapBuffers вернул FALSE\n");
            running = FALSE;
            break;
        }

        Sleep(16);
    }

    printf("[STEP] cleanup\n");

    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);

    printf("[DONE]\n");
    return 0;
}
