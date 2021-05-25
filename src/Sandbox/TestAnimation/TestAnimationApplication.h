#pragma once
#include "ModelScript.h"

#include "../CameraScript.h"

#include "../../Core/Alias.h"
#include "../../Core/Application.h"
#include "../../Animation/AnimatedModel.h"
#include "../../Graphics/Shader.h"
#include "../../Scene/CameraFPS.h"
#include "../../Graphics/IDrawable.h"

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


    TestAnimationApplication()
        : Application(WIDTH, HEIGHT, "Test Animation")
    {
        SetupeDrawable();
        SetupCamera();
    }

    void SetupCamera()
    {
        _camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 5));
        _camera->SetProjectionParameters((float)WIDTH / (float)HEIGHT, 60.0f);
        auto cameraScript = CreateRef<CameraScript>();
        BehaviourScript::Link(_camera, cameraScript);
        _scriptables.push_back(std::static_pointer_cast<ScriptableEntity>(_camera));
    }

    void SetupeDrawable()
    {
        // Loading assets
        _shader = CreateUnique<Shader>("res/shaders/animation/shader.vs",
                                       "res/shaders/animation/shader.fs");
        auto mesh = CreateRef<AnimatedMesh>("res/meshes/man.dae");

        // Creating drawable entities
        auto model = CreateRef<AnimatedModel>(mesh);
        auto modelScript = CreateRef<ModelScript>();
        BehaviourScript::Link(model, modelScript);

        _scriptables.push_back(model);
        _drawableModel = std::static_pointer_cast<IDrawable>(model);
    }

    void Render() override
    { 
        _drawableModel->Draw(*_shader, *_camera); 
    }

    void Start() override
    {
        for (auto& scriptable : _scriptables) scriptable->Start();
    }

    void Update(float deltaTime) override
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) Shutdown();
        for (auto& scriptable : _scriptables) scriptable->Update(deltaTime);
    }
};

}