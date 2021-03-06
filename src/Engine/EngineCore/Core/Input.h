#pragma once

#include <glm/glm.hpp>
#include "Window.h"

namespace FQW {

class Input
{
public:
    Input() = delete;

    static void Initialize(Window& window);

    static bool IsKeyPressed(short keycode);
    static bool IsMouseButtonPressed(short mouseButtonCode);

    static glm::vec2 GetCursorPosition();

    static double GetTime_s();
    static double GetTime_ms();

private:
    static GLFWwindow* s_Window;
};


} // namespace FQW