#pragma once
#include <Engine.h>
#include "../../Scripts/ApplicationScript.h"
#include "../../Scripts/ModelScript.h"


namespace FQW::MeshShaderDemo {

class App_MeshShaderDemo : public Application
{
private:
    const string SHADERS_DIRECTORY = s_SolutionDirectory + "src\\Demo\\Apps\\MeshShaderDemo\\Shaders\\";
    const string MODELS_DIRECTORY  = s_SolutionDirectory + "res\\meshes\\working\\";

    Unique<ShaderPipeline> m_ClassicPipeline;
    Unique<ShaderPipeline> m_MeshShaderPipeline;
    Ref<Model> m_Model;
    Ref<Animator> m_Animator;

    struct {
        bool useClassicPipeline = true;
        bool showMeshlets = false;
        bool wireframe = false;
        bool backfaceCulling = false;
    } m_RenderConfig;


public:
    App_MeshShaderDemo()
        : Application(1280, 720, "Test Mesh Shader")
    {
        // Load model
        ModelLoader loader(MeshOptimizer(true, true, true));
        m_Model = loader.LoadModel(MODELS_DIRECTORY + "cubes.obj");

        auto modelScript = CreateRef<MainApp::ModelScript>();
        Script::Connect(m_Model, modelScript);
        RegisterScriptableEntity(m_Model);

        // Setup animator
        m_Animator = m_Model->GetAnimator();
        RegisterUpdatableEntity(m_Animator);

        // Load shaders
        m_ClassicPipeline = CreateUnique<ClassicShaderPipeline>(SHADERS_DIRECTORY + "vertex.vert",
                                                                SHADERS_DIRECTORY + "fragment.frag");

        m_MeshShaderPipeline = CreateUnique<MeshShaderPipeline>(SHADERS_DIRECTORY + "shader_multithread.mesh",
                                                                SHADERS_DIRECTORY + "fragment.frag");    
    }

    void Render() override
    {
        if (m_RenderConfig.wireframe) {
            glPolygonMode(GL_FRONT, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT, GL_FILL);
        }

        if (m_RenderConfig.backfaceCulling)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        if (m_RenderConfig.useClassicPipeline) {
            m_Model->Draw(*m_ClassicPipeline, *m_Camera);
        }
        else {
            m_Model->Draw(*m_MeshShaderPipeline, *m_Camera);
        }
    }

    void DrawUI() override
    {
        ImGui::Begin(u8"Производительность");
        ImGui::SetWindowFontScale(1.0);
        ImGui::Text(u8"Частота кадров %.3f", 1 / m_DeltaTime * 1000);
        ImGui::Text(u8"Время композиции кадра %.3f", m_DeltaTime);
        ImGui::End();

        ImGui::Begin(u8"Камера");
        ImGui::SetWindowFontScale(1.0);
        ImGui::Text(u8"Позиция [%.3f %.3f %.3f]", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
        ImGui::Text(u8"Ориентация: [%.3f %.3f, %.3f]", m_Camera->GetFront().x, m_Camera->GetFront().y, m_Camera->GetFront().z);
        ImGui::End();

        ImGui::Begin(u8"Модель");
        ImGui::SetWindowFontScale(1.0);

        ImGui::Text(u8"Позиция");
        ImGui::SliderFloat("x", &m_Model->Transform.position.x, -3.0f, 3.0f);
        ImGui::SliderFloat("y", &m_Model->Transform.position.y, -3.0f, 3.0f);
        ImGui::SliderFloat("z", &m_Model->Transform.position.z, -3.0f, 3.0f);

        ImGui::Text(u8"Ориентация");
        ImGui::SliderFloat("rx", &m_Model->Transform.rotation.x, -180.0f, 180.0f);
        ImGui::SliderFloat("ry", &m_Model->Transform.rotation.y, -180.0f, 180.0f);
        ImGui::SliderFloat("rz", &m_Model->Transform.rotation.z, -180.0f, 180.0f);
        ImGui::End();

        ImGui::Begin(u8"Рендер");
        ImGui::Checkbox(u8"Использовать классический конвейер", &m_RenderConfig.useClassicPipeline);
        ImGui::Checkbox(u8"Показывать кластеры", &m_RenderConfig.showMeshlets);
        ImGui::Checkbox(u8"Каркас", &m_RenderConfig.wireframe);
        ImGui::Checkbox(u8"Фильтровать задние грани", &m_RenderConfig.backfaceCulling);
        ImGui::End();
    }


    void Init()
    {
        auto applicationScript = CreateRef<ApplicationScript>();
        Script::Connect(shared_from_this(), applicationScript);
        RegisterUpdatableEntity(applicationScript);
    }
};

}