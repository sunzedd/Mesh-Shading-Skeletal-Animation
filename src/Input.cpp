#include "Input.h"

namespace FQW {

GLFWwindow* Input::s_Window;

void Input::Initialize(Window& window)
{
    s_Window = (GLFWwindow*)window.GetNativeWindow();
    if (!s_Window)
    {
        FQW_CRITICAL("GLFWwindow is NULL while Input initializing");
        throw std::runtime_error("GLFWwindow is NULL while Input initializing");
    }

    FQW_TRACE("Initialized Input");
}


bool Input::IsKeyPressed(short keycode)
{
    return (GLFW_PRESS == glfwGetKey(s_Window, keycode));
}


bool Input::IsMouseButtonPressed(short mouseButtonCode)
{
    return (GLFW_PRESS == glfwGetMouseButton(s_Window, mouseButtonCode));
}


std::pair<int, int> Input::GetCursorPosition()
{
    double x, y;
    glfwGetCursorPos(s_Window, &x, &y);
    return std::make_pair(floor(x), floor(y));
}


double Input::GetTime_s()
{
    return glfwGetTime();
}


double Input::GetTime_ms()
{
    return glfwGetTime() / 1000.0f;
}

}