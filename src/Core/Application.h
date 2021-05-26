#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "../Graphics/OpenGL_Utils.h"

#include "Window.h"
#include "Input.h"
#include "Alias.h"

namespace FQW {

static const ImWchar ranges[] =
{
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x0400, 0x044F, // Cyrillic
    0,
};

class Application
{
public:
    Application(int width=800, int height=600, std::string title="Application");
    virtual ~Application();

    void Run();

protected:
    virtual void Update(float deltaTime) { }
    virtual void Render() { }
    virtual void DrawUI() { }

    virtual void Start() { }
    virtual void Shutdown();

    virtual void SetupGraphicsPipeline();

private:
    bool CheckGraphicsRequirements();
    void SetupImgui();

protected:
    Unique<Window> m_Window;
    bool m_IsRunning;
    float m_LastFrameTime = 0.0f;

    ImFontConfig m_FontConfig;
};

} // namespace FQW