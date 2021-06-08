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
    SetupShader();

    auto appScript = CreateRef<ApplicationScript>();
    Script::Connect(shared_from_this(), appScript);
    RegisterUpdatableEntity(appScript);
}


void App_Main::SetupShader()
{
    _ShaderPipeline = CreateUnique<ClassicShaderPipeline>(
        s_SolutionDirectory + "res\\shaders\\animation\\shader.vs",
        s_SolutionDirectory + "res\\shaders\\animation\\shader.fs"
    );
}


void App_Main::SetupScene()
{
    ModelLoader loader(MeshOptimizer(false, false, false));
    loader.LoadModel(MODEL_FILEPATH);
    FQW_TRACE("Loaded animated model from {}", MODEL_FILEPATH);

    auto modelScript = CreateRef<FQW::MainApp::ModelScript>();
    Script::Connect(_Model, modelScript);

    _Animator = _Model->GetAnimator();

    RegisterScriptableEntity(_Model);
    RegisterUpdatableEntity(_Model);
    RegisterUpdatableEntity(_Animator);
}


void App_Main::Render()
{
    _Model->Draw(*_ShaderPipeline, *m_Camera);
}


void App_Main::DrawUI()
{
    ImGui::Begin(u8"Производительность");
    ImGui::SetWindowFontScale(1.3);
    ImGui::Text(u8"Частота кадров %.3f", 1 / m_DeltaTime * 1000);
    ImGui::Text(u8"Время композиции кадра %.3f", m_DeltaTime);
    ImGui::End();

    ImGui::Begin(u8"Камера");
    ImGui::SetWindowFontScale(1.3);
    ImGui::Text(u8"Позиция [%.3f %.3f %.3f]", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
    ImGui::Text(u8"Ориентация: [%.3f %.3f, %.3f]", m_Camera->GetFront().x, m_Camera->GetFront().y, m_Camera->GetFront().z);
    ImGui::End();

    ImGui::Begin(u8"Модель");
    ImGui::SetWindowFontScale(1.3);
    
    ImGui::Text(u8"Позиция");
    ImGui::SliderFloat("x", &_Model->Transform.position.x, -3.0f, 3.0f);
    ImGui::SliderFloat("y", &_Model->Transform.position.y, -3.0f, 3.0f);
    ImGui::SliderFloat("z", &_Model->Transform.position.z, -3.0f, 3.0f);

    ImGui::Text(u8"Ориентация");
    ImGui::SliderFloat("rx", &_Model->Transform.rotation.x, -180.0f, 180.0f);
    ImGui::SliderFloat("ry", &_Model->Transform.rotation.y, -180.0f, 180.0f);
    ImGui::SliderFloat("rz", &_Model->Transform.rotation.z, -180.0f, 180.0f);
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