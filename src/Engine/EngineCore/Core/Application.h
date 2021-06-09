#pragma once
//#include <string>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "../../Scripts/FPSCameraScript.h"

#include "../Graphics/OpenGL_Utils.h"
#include "../Scene/Script.h"
#include "../Scene/PerspectiveCamera.h"

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



class Application : public ScriptableEntity, public std::enable_shared_from_this<Application>
{
public:
    Application(int width=800, int height=600, const string& title="Application");
    virtual ~Application();
    
    virtual void Init() { };
    void Run();
    virtual void Shutdown();

protected:    
    virtual void Render() { }
    virtual void DrawUI() { }

    virtual void SetupGraphicsPipeline();

    void RegisterUpdatableEntity(Ref<IUpdatable> updatable);
    void RegisterScriptableEntity(Ref<ScriptableEntity> scriptable);

private:
    void Start();
    void Update(float deltaTime);

    bool CheckGraphicsRequirements();
    void SetupDefaultCameraFPS();
    void SetupImgui();

protected:
    string m_Name;
    Unique<Window> m_Window;

    bool m_IsRunning;
    float m_LastFrameTime = 0.0f;
    float m_DeltaTime;

    ImFontConfig m_FontConfig;

    Ref<PerspectiveCamera> m_Camera;
    vector<Ref<IUpdatable>> m_UpdatableEntities;
    vector<Ref<ScriptableEntity>> m_ScriptableEntities;
};

} // namespace FQW