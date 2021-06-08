#include "Application.h"

namespace FQW {


Application::Application(int width, int height, const string& title)
    : 
    m_Name(title)
{
    Logger::Initialize();
    m_Window = CreateUnique<Window>(width, height, m_Name.c_str());
    Input::Initialize(*m_Window);

    CheckGraphicsRequirements();
    SetupGraphicsPipeline();
    SetupImgui();
    SetupDefaultCameraFPS();
}


Application::~Application() { }


void Application::Start()
{
    if (HasScript())
    {
        m_BehaviourScript->Start();
    }

    for (auto& entity : m_ScriptableEntities) 
    {
        entity->Start();
    }
}


void Application::Run()
{
    FQW_INFO("Running application: {}", m_Name);

    m_IsRunning = true;

    Start();

    while (m_IsRunning)
    {
        if (m_Window->IsClosed())
        {
            m_IsRunning = false;
            continue;
        }

        const float currentTime = Input::GetTime_ms();
        m_DeltaTime = currentTime - m_LastFrameTime;
        m_LastFrameTime = currentTime;

        Update(m_DeltaTime);

        m_Window->Clear();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Render();
        DrawUI();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        m_Window->Update();
    }
}


void Application::Update(float deltaTime)
{
    for (auto& entity : m_UpdatableEntities)
    {
        entity->Update(deltaTime);
    }
}


void Application::Shutdown()
{
    m_IsRunning = false;
    m_Window->Close();
}


void Application::SetupGraphicsPipeline()
{
    glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}


void Application::SetupDefaultCameraFPS()
{
    m_Camera = CreateRef<PerspectiveCamera>(glm::vec3(0, 0, 20));
    m_Camera->SetProjectionParameters((float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 60.0f);
    auto cameraScript = CreateRef<FPSCameraScript>();
    Script::Connect(m_Camera, cameraScript);

    RegisterUpdatableEntity(m_Camera);
    RegisterScriptableEntity(m_Camera);
}


void Application::SetupImgui()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)m_Window->GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 16.0f, &m_FontConfig, ranges);
}


bool Application::CheckGraphicsRequirements()
{
    bool allRequrementsSatisfied = true;

    std::string gpuName = FQW::OpenGL_GetGraphicsDeviceInfo();
    FQW_INFO(gpuName);

    std::pair<int,int> openglVersion = FQW::OpenGL_GetVersion();
    FQW_INFO("OpenGL version: {}.{}", openglVersion.first, openglVersion.second);

    std::string requiredExtensionName = "GL_NV_mesh_shader";
    if (FQW::OpenGL_CheckExtensionSupport(requiredExtensionName))
    {
        FQW_INFO("Extension is supported: {}", requiredExtensionName);
    }
    else
    {
        FQW_CRITICAL("Extension is not supported: {}", requiredExtensionName);
        allRequrementsSatisfied = false;
    }

    return allRequrementsSatisfied;
}


void Application::RegisterUpdatableEntity(Ref<IUpdatable> updatable)
{
    m_UpdatableEntities.push_back(updatable);
}


void Application::RegisterScriptableEntity(Ref<ScriptableEntity> scriptable)
{
    m_ScriptableEntities.push_back(scriptable);
    RegisterUpdatableEntity(scriptable);
}


} // namespace FQW