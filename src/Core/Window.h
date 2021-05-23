#pragma once
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.h"

///
///    Wrapper over GLFW3 window
///

namespace FQW {

class Window
{
public:
    Window();
    Window(int width, int height, const char *title);
    ~Window();

    int GetWidth() const { return m_Size.width; }
    int GetHeight() const { return m_Size.height; }
    const char *GetTitle() const { return m_Title; }
    void* GetNativeWindow() { return (void*)m_Window; }

    void Clear();
    void Update();
    void Close();

    bool IsClosed();

private:
    bool _InitializeGlfw();
    void _ShutdownGlfw();

    void _Create(int width, int height, const char* title);

private:
    struct {
        int width;
        int height;
    } m_Size;

    const char *m_Title;
    GLFWwindow *m_Window;
};

} // namespace VRV
