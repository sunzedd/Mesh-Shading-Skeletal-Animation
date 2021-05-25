#pragma once
#include "ModelScript.h"

#include "../CameraScript.h"

#include "../../Core/Alias.h"
#include "../../Core/Application.h"

#include "../../Animation/AnimatedModel.h"
#include "../../Animation/Loader.h"

#include "../../Scene/CameraFPS.h"


namespace FQW {

class TestAnimationApplication : public Application
{
public:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;

    Unique<Shader> _shader;
    Ref<CameraFPS> _camera;

    Ref<IDrawable> _drawableModel;

    std::vector<Ref<ScriptableEntity>> _scriptables;

    Ref<Animator> _animator;


    TestAnimationApplication()
        : Application(WIDTH, HEIGHT, "Test Animation")
    {
        SetupDrawable();
        SetupCamera();
    }

    void SetupCamera()
    {
        _camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 5));
        _camera->SetProjectionParameters((float)WIDTH / (float)HEIGHT, 60.0f);
        auto cameraScript = CreateRef<CameraScript>();
        Script::Link(_camera, cameraScript);
        _scriptables.push_back(std::static_pointer_cast<ScriptableEntity>(_camera));
    }

    void SetupDrawable()
    {
        // Loading assets
        _shader = CreateUnique<Shader>("res/shaders/animation/shader.vs",
                                       "res/shaders/animation/shader.fs");
        
        auto model = Loader::LoadAnimatedModel("res/meshes/man.dae");
        FQW_TRACE("Loaded animated model from 'res/meshes/man.dae'");

        _animator = model->GetAnimator();

        auto modelScript = CreateRef<ModelScript>();
        Script::Link(model, modelScript);

        _scriptables.push_back(model);
        _drawableModel = std::static_pointer_cast<IDrawable>(model);
    }

    void Render() override
    { 
        _drawableModel->Draw(*_shader, *_camera); 
    }

    void Start() override
    {
        for (auto& scriptable : _scriptables)
            scriptable->Start();
    }

    void Update(float deltaTime) override
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) Shutdown();

        for (auto& scriptable : _scriptables)
            scriptable->Update(deltaTime);

        _animator->Update(deltaTime);
    }
};

}