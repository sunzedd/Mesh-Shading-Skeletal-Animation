#include "App_Main.h"

namespace FQW::Main {

App_Main::App_Main()
    : Application(WIDTH, HEIGHT, "Test Animation")
{
    glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT, GL_LINE);
}


void App_Main::Init()
{
    SetupScene();
    SetupCamera();
    SetupShader();
}


void App_Main::SetupCamera()
{
    _camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 20));
    _camera->SetProjectionParameters((float)WIDTH / (float)HEIGHT, 60.0f);
    auto cameraScript = CreateRef<CameraScript>();
    Script::Link(_camera, cameraScript);
    _scriptables.push_back(std::static_pointer_cast<ScriptableEntity>(_camera));
}


void App_Main::SetupShader()
{
    _shaderPipeline = CreateUnique<ClassicShaderPipeline>(
        s_SolutionDirectory + "res\\shaders\\animation\\shader.vs",
        s_SolutionDirectory + "res\\shaders\\animation\\shader.fs"
    );
}


void App_Main::SetupScene()
{
    _model = ModelLoader::LoadModel(MODEL_FILEPATH);
    FQW_TRACE("Loaded animated model from {}", MODEL_FILEPATH);

    _animator = _model->GetAnimator();

    auto modelScript = CreateRef<FQW::MainApp::ModelScript>();
    Script::Link(_model, modelScript);

    _scriptables.push_back(_model);
}


void App_Main::Render()
{
    _model->Draw(*_shaderPipeline, *_camera);
}


void App_Main::Start()
{
    for (auto& scriptable : _scriptables) {
        scriptable->Start();
    }
}

void App_Main::Update(float deltaTime)
{
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) Shutdown();

    for (auto& scriptable : _scriptables)
        scriptable->Update(deltaTime);

    _animator->Update(deltaTime);
}

void App_Main::DrawUI()
{
    ImGui::Begin(u8"Производительность");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text(u8"Время композиции кадра %.3f", m_DeltaTime);
    ImGui::SetWindowFontScale(1);
    ImGui::End();
}



#if LOG_ANIMATION_DATA
void App_Main::LogSkeletonHierarchy(const Bone& bone, std::string logOffset) {
    if (bone.name.empty()) {
        FQW_WARN("{} unnamed {}", logOffset, bone.name);
    }
    else {
        FQW_TRACE("{} {}", logOffset, bone.name);
    }

    for (const auto& child : bone.children) {
        LogSkeletonHierarchy(child, logOffset + "   ");
    }
}


void App_Main::LogAnimationTransformations(Ref<Animation> animation) {
    for (const auto& transform : animation->boneTransforms) {
        const std::string& name = transform.first;
        if (name.empty()) {
            FQW_WARN("[LogAnimationTransformations]: transformation unnamed {}", name);
        }
    }
}
#endif

} // namespace FQW