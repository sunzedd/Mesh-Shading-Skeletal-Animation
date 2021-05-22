#pragma once
#include "../Graphics/OpenGL_Utils.h"

#include "Window.h"
#include "Input.h"
#include "Alias.h"

namespace FQW {

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

private:
    bool CheckGraphicsRequirements();

protected:
    Unique<Window> m_Window;
    bool m_IsRunning;
    float m_LastFrameTime = 0.0f;
};

} // namespace FQW