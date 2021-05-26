#include "MainApp.h"

namespace FQW {

MainApp::MainApp()
    : Application(WIDTH, HEIGHT, "Test Animation")
{
    SetupDrawable();
    SetupCamera();
}


void MainApp::SetupCamera()
{
    _camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 20));
    _camera->SetProjectionParameters((float)WIDTH / (float)HEIGHT, 60.0f);
    auto cameraScript = CreateRef<CameraScript>();
    Script::Link(_camera, cameraScript);
    _scriptables.push_back(std::static_pointer_cast<ScriptableEntity>(_camera));
}


void MainApp::SetupDrawable()
{
    // Loading assets
    _shader = CreateUnique<Shader>(
        "res/shaders/animation/shader.vs",
        "res/shaders/animation/shader.fs"
        );

    _model = ModelLoader::LoadModel(MODEL_FILEPATH);

    FQW_TRACE("Loaded animated model from {}", MODEL_FILEPATH);
    FQW_TRACE("\tMeshes count: {}", _model->GetMeshes().size());
    FQW_TRACE("\tAnimations count: {}", _model->GetAnimations().size());
    _animator = _model->GetAnimator();

    FQW_INFO("Skeleton hierarchy");
    LogSkeletonHierarchy(_animator->GetSkeleton());

    for (const auto& anim : _model->GetAnimations()) {
        LogAnimationTransformations(anim);
    }

    auto modelScript = CreateRef<ModelScript>();
    Script::Link(_model, modelScript);

    _scriptables.push_back(_model);
    _drawableModel = _model;
}


void MainApp::LogSkeletonHierarchy(const Bone& bone, std::string logOffset) {
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


void MainApp::LogAnimationTransformations(Ref<Animation> animation) {
    for (const auto& transform : animation->boneTransforms) {
        const std::string& name = transform.first;
        if (name.empty()) {
            FQW_WARN("[LogAnimationTransformations]: transformation unnamed {}", name);
        }
    }
}


void MainApp::Render()
{
    _drawableModel->Draw(*_shader, *_camera);
}


void MainApp::Start()
{
    for (auto& scriptable : _scriptables) {
        scriptable->Start();
    }
}

void MainApp::Update(float deltaTime)
{
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) Shutdown();

    for (auto& scriptable : _scriptables)
        scriptable->Update(deltaTime);

    _animator->Update(deltaTime);
}

} // namespace FQW