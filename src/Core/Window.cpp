#include "Window.h"

namespace FQW {

Window::Window()
    :
    m_Size{800, 600},
    m_Title("FQW Window")
{
    _Create(m_Size.width, m_Size.height, m_Title);
}


Window::Window(int width, int height, const char* title) 
    : 
    m_Size{width, height},
    m_Title(title)
{
    _Create(m_Size.width, m_Size.height, m_Title);
}


Window::~Window()
{
    _ShutdownGlfw();
}


void Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Window::Update()
{
    glfwSwapBuffers(glfwGetCurrentContext());
    glfwPollEvents();
}


void Window::Close()
{
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}


bool Window::IsClosed()
{
    return static_cast<bool> (glfwWindowShouldClose(m_Window));
}


bool Window::_InitializeGlfw()
{
    FQW_TRACE("Initializing glfw3");
    return static_cast<bool>(glfwInit());
}


void Window::_ShutdownGlfw()
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
    }

    glfwTerminate();
    FQW_TRACE("Terminated glfw3");
}


void Window::_Create(int width, int height, const char* title)
{
    if (!_InitializeGlfw())
    {
        throw std::runtime_error("Failed to inititalize glfw3");
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    FQW_TRACE("Creating window {}x{} title: {} ...", width, height, title);

    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_Window)
    {
        FQW_CRITICAL("Failed to create glfw3 window");

        _ShutdownGlfw();
        throw std::runtime_error("Failed tot create glfw3 window");
    }

    glfwMakeContextCurrent(m_Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
}

} // namespace FQW