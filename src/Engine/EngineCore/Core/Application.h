#pragma once
//#include <string>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "../../CameraScript.h"

#include "../Graphics/OpenGL_Utils.h"
#include "../Scene/Script.h"
#include "../Scene/CameraFPS.h"

#include "Alias.h"
#include "Window.h"
#include "Input.h"


namespace FQW {

static const ImWchar ranges[] =
{
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x0400, 0x044F, // Cyrillic
    0,
};

static const std::string s_SolutionDirectory = SOLUTION_DIR;



class Application
{
public:
    Application(int width=800, int height=600, std::string title="Application");
    virtual ~Application();

    void Run();


protected:
    void Start();
    void Update(float deltaTime);
    
    virtual void Render() { }
    virtual void DrawUI() { }

    virtual void Shutdown();

    virtual void SetupGraphicsPipeline();

    void RegisterUpdatableEntity(Ref<IUpdatable> updatable);
    void RegisterScriptableEntity(Ref<ScriptableEntity> scriptable);


private:
    bool CheckGraphicsRequirements();
    void SetupDefaultCameraFPS();
    void SetupImgui();


protected:
    Unique<Window> m_Window;
    bool m_IsRunning;
    float m_LastFrameTime = 0.0f;
    float m_DeltaTime;

    ImFontConfig m_FontConfig;

    string m_Name = "Application";

    Ref<CameraFPS> m_Camera;
    vector<Ref<IUpdatable>> m_UpdatableEntities;
    vector<Ref<ScriptableEntity>> m_ScriptableEntities;
};

} // namespace FQW