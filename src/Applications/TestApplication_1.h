#pragma once

#include "../Core/Application.h"
#include "../Graphics/StaticMesh.h"
#include "../Scene/Object.h"

namespace FQW {


class OrthographicCamera : public ICamera
{
public:
    OrthographicCamera(int width, int height)
    {
        m_Ratio = (float)width / (float)height;
    }

    glm::mat4 GetViewMatrix() const override
    {
        return glm::mat4(1.0f);
    }

    glm::mat4 GetProjectionMatrix() const override
    {
        return glm::ortho(-m_Ratio, m_Ratio, -1.0f, 1.0f);
    }

private:
    float m_Ratio;
};


class TestApplication_1 : public Application
{
private:
    std::vector<Ref<IDrawable>> m_Scene;
    Unique<Shader> m_Shader;
    Unique<ICamera> m_Camera;

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

        auto staticMesh = CreateRef<StaticMesh>(vertices, indices);
        auto object = CreateRef<Object>();
        object->PushDrawable(staticMesh);

        m_Scene.push_back(object);

        m_Shader = CreateUnique<Shader>("res/shaders/test_1.vs", "res/shaders/test_1.fs");
        m_Camera = CreateUnique<OrthographicCamera>(m_Window->GetWidth(), m_Window->GetHeight());
    }

private:
    void Update(float deltaTime) override
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
        {
            Shutdown();
        }
    }

    void Render() override
    {
        for (auto object : m_Scene)
        {
            object->Draw(*m_Shader, *m_Camera);
        }
    }

    void DrawUI() override
    {

    }
};


} // namespace FQW