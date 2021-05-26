#pragma once
#include "../CameraScript.h"

#include "../../Core/Alias.h"
#include "../../Core/Application.h"
#include "../../Scene/CameraFPS.h"
#include "../../AnimationMultiple/ModelLoader.h"



namespace FQW {

class TestAnimationMultipleApplication : public Application
{
public:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;
    const std::string MODEL_FILEPATH = "res/meshes/argon.dae";

    Unique<Shader> _shader;
    Ref<CameraFPS> _camera;

    Ref<IDrawable> _drawableModel;
    Ref<Model> _model;
    Ref<Animator> _animator;

    std::vector<Ref<ScriptableEntity>> _scriptables;


public:
    TestAnimationMultipleApplication()
        : Application(WIDTH, HEIGHT, "Test Animation")
    {
        SetupDrawable();
        SetupCamera();
    }

    void SetupCamera()
    {
        _camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 20));
        _camera->SetProjectionParameters((float)WIDTH / (float)HEIGHT, 60.0f);
        auto cameraScript = CreateRef<CameraScript>();
        Script::Link(_camera, cameraScript);
        _scriptables.push_back(std::static_pointer_cast<ScriptableEntity>(_camera));
    }

    void SetupDrawable()
    {
        // Loading assets
        _shader = CreateUnique<Shader>(
            "res/shaders/animation/shader.vs",
            "res/shaders/animation/shader.fs"
        );

        _model = ModelLoader::LoadModel(MODEL_FILEPATH);
        FQW_TRACE("Loaded animated model from {}", MODEL_FILEPATH);
        FQW_TRACE("\tMeshes count: {}", _model->meshes.size());
        FQW_TRACE("\tAnimations count: {}", _model->animations.size());
        _animator = _model->animator;

        FQW_INFO("Skeleton hierarchy");
        LogSkeletonHierarchy(_animator->GetSkeleton());
        
        for (const auto& anim : _model->animations) {
            LogAnimationTransformations(anim);
        }
        FQW_TRACE("");

        _drawableModel = _model;
    }

    void LogSkeletonHierarchy(const Bone& bone, std::string logOffset = "") {
        if (bone.name.empty()) {
            FQW_WARN("{} unnamed {}", logOffset, bone.name);
        } else {
            FQW_TRACE("{} {}", logOffset, bone.name);
        }

        for (const auto& child : bone.children) {
            LogSkeletonHierarchy(child, logOffset + "   ");
        }
    }

    void LogAnimationTransformations(Ref<Animation> animation) {
        for (const auto& transform : animation->boneTransforms) {
            const std::string& name = transform.first;
            if (name.empty()) {
                FQW_WARN("[LogAnimationTransformations]: transformation unnamed {}", name);
            }
        }
    }


    void Render() override
    {
        _drawableModel->Draw(*_shader, *_camera);
    }

    void Start() override
    {
        for (auto& scriptable : _scriptables) {
            scriptable->Start();
        }
    }

    void Update(float deltaTime) override
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) Shutdown();

        for (auto& scriptable : _scriptables)
            scriptable->Update(deltaTime);

        _animator->Update(deltaTime);

        if (!_animator->IsPlaying())
        {
            if (Input::IsKeyPressed(GLFW_KEY_1)) {
                _animator->PlayAnimation(_model->animations[0]);  FQW_INFO("Playing animation 0");
            }
            else if (Input::IsKeyPressed(GLFW_KEY_2)) {
                _animator->PlayAnimation(_model->animations[1]);  FQW_INFO("Playing animation 1");
            }
            else if (Input::IsKeyPressed(GLFW_KEY_3)) {
                _animator->PlayAnimation(_model->animations[2]);  FQW_INFO("Playing animation 2");
            }
        }
    }
};

}