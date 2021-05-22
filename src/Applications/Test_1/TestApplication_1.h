#pragma once

#include <memory>

#include "../../Core/Application.h"
#include "../../Graphics/StaticMesh.h"

#include "Scripts.h"

namespace FQW {

/*
class TestCamera : public ICamera
{
public:
    const glm::mat4& GetViewMatrix() const override
    {
        return glm::mat4(1.0f);
    }

    const glm::mat4& GetProjectionMatrix() const override
    {
        return glm::ortho(-1.6f, 1.6f, -1.0f, 1.0f);
    }
};*/


class TestApplication_1 : public Application
{
private:
    std::vector<Ref<IDrawable>> m_Drawables;
    std::vector<Ref<ScriptableEntity>> m_Scriptables;
    Unique<Shader> m_Shader;
    Ref<ICamera> m_Camera;

public:
    TestApplication_1()
        :
        Application(1536, 960, "Test Application 1")
    {
        std::vector<StaticMesh::Vertex> vertices = {
            { {-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 4.0f}, {1.0f, 4.0f} },
            { { 0.5f,  0.5f, 0.0f}, {2.0f, 0.0f, 3.0f}, {2.0f, 3.0f} },
            { {-0.5f, -0.5f, 0.0f}, {3.0f, 0.0f, 2.0f}, {3.0f, 2.0f} },
            { { 0.5f, -0.5f, 0.0f}, {4.0f, 0.0f, 1.0f}, {4.0f, 1.0f} }
        };

        std::vector<unsigned int> indices = { 0, 1, 2, 1, 3, 2 };

        Ref<StaticMesh> staticMesh = CreateRef<StaticMesh>(vertices, indices);
        Ref<Object> object = CreateRef<Object>();
        Ref<ObjectScript> objectScript = CreateRef<ObjectScript>();
        BehaviourScript::Link(object, objectScript);
        
        object->PushDrawable(staticMesh);
        m_Drawables.push_back(object);
        m_Scriptables.push_back(object);

        m_Shader = CreateUnique<Shader>("res/shaders/test_1.vs", "res/shaders/test_1.fs");

        m_Camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 10));
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