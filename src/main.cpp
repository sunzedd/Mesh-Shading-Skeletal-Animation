#include "Core/Window.h"
#include "Core/Input.h"
#include "Scene/CameraFPS.h"
#include "Scene/Object.h"
#include "Graphics/Mesh.h"


#if 1
/// Test All

using namespace FQW;

class TestCamera : public ICamera
{
public:
    glm::mat4 GetViewMatrix() const override
    {
        return glm::mat4(1.0f);
    }

    glm::mat4 GetProjectionMatrix() const override
    {
        return glm::ortho(-16.0f / 10.0f, 16.0f / 10.0f, -1.0f, 1.0f);
    }
};

class ObjectImpl : public Object
{
public:
    ObjectImpl(IDrawable* drawable)
        :
        Object()
    {
        m_Drawables.push_back(drawable);
    }

    void Update(float deltaTime) override
    {
        this->Rotation += deltaTime;
    }
};


int main()
{
    Logger::Initialize();
    Window w(1680, 1050, "Test");
    Input::Initialize(w);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

    const auto gpuName = FQW::OpenGL_GetGraphicsDeviceInfo();
    FQW_INFO(gpuName);

    const auto openglVersion = FQW::OpenGL_GetVersion();
    FQW_INFO("OpenGL version: {}.{}", openglVersion.first, openglVersion.second);

    std::string requiredExtensionName = "GL_NV_mesh_shader";
    if (FQW::OpenGL_CheckExtensionSupport(requiredExtensionName))
    {
        FQW_INFO("Extension is supported: {}", requiredExtensionName);
    }
    else
    {
        FQW_CRITICAL("Extension is not supported: {}", requiredExtensionName);
    }

    std::vector<Vertex> vertices = {
        { {-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 4.0f}, {1.0f, 4.0f} },
        { { 0.5f,  0.5f, 0.0f}, {2.0f, 0.0f, 3.0f}, {2.0f, 3.0f} },
        { {-0.5f, -0.5f, 0.0f}, {3.0f, 0.0f, 2.0f}, {3.0f, 2.0f} },
        { { 0.5f, -0.5f, 0.0f}, {4.0f, 0.0f, 1.0f}, {4.0f, 1.0f} }
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 1, 3, 2 };

    Mesh mesh(vertices, indices);
    ObjectImpl o(&mesh);

    Shader shader("res/shaders/test_1.vs", "res/shaders/test_1.fs");
    TestCamera camera;

    while (!w.IsClosed())
    {
        o.Update(glfwGetTime() * 0.001);

        w.Clear();
        o.Draw(shader, camera);
        w.Update();
    }
}

#endif
