#pragma once

#include <memory>

#include "../../Core/Application.h"
#include "../../Scene/Model/StaticModel.h"

#include "../CameraScript.h"

namespace FQW {


class TestApplication_2 : public Application
{
private:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;

    std::vector<Ref<IDrawable>> m_Drawables;
    std::vector<Ref<ScriptableEntity>> m_Scriptables;
    Unique<Shader> m_Shader;
    Ref<CameraFPS> m_Camera;

public:
    TestApplication_2()
        :
        Application(WIDTH, HEIGHT, "Test Application 2")
    {
        auto animatedModel = CreateRef<StaticModel>("res/meshes/M4A1.dae");
        m_Drawables.push_back(animatedModel);

        m_Shader = CreateUnique<Shader>("res/shaders/test_1.vs", "res/shaders/test_1.fs");

        m_Camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 20));
        m_Camera->SetProjectionParameters((float)WIDTH / (float)HEIGHT, 60.0f);
        Ref<CameraScript> cameraScript = CreateRef<CameraScript>();
        BehaviourScript::Link(m_Camera, cameraScript);

        m_Scriptables.push_back(m_Camera);
    }


private:
    void Start() override
    {
        for (auto& object : m_Scriptables)
        {
            object->Start();
        }
    }

    void Update(float deltaTime) override
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
        {
            Shutdown();
        }

        for (auto& object : m_Scriptables)
        {
            object->Update(deltaTime);
        }
    }

    void Render() override
    {
        for (auto& object : m_Drawables)
        {
            object->Draw(*m_Shader, *m_Camera);
        }
    }

    void DrawUI() override
    {

    }
};


} // namespace FQW