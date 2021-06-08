#pragma once
#include "../Main/App_Main.h"
#include "NewTypes.h"


namespace FQW::TestMeshShaders {

struct App_TestMeshShaderFullVertex : public Main::App_Main
{
    const string LOCAL_SHADERS_DIRECTORY = s_SolutionDirectory +  "src\\Demo\\Apps\\TestMeshShadersFullVertex\\shaders\\";
    const string GLOBAL_SHADERS_DIRECTORY = s_SolutionDirectory + "res\\shaders\\";

    Model_ model;
    Unique<ShaderPipeline> m_ClassicShaderPipeline;

    struct {
        bool useClassicPipeline = true;
        bool showMeshlets = false;
        bool wireframe = true;
        bool backfaceCulling = false;
    } m_RenderConfig;


    App_TestMeshShaderFullVertex()
        : App_Main() 
    { 
        m_Name = "Test Mesh Shader";
        MODEL_FILEPATH = s_SolutionDirectory + "res\\meshes\\working\\cubes.obj";

        m_ClassicShaderPipeline = CreateUnique<ClassicShaderPipeline>(
            GLOBAL_SHADERS_DIRECTORY + "animation\\shader.vs",
            GLOBAL_SHADERS_DIRECTORY + "animation\\shader.fs");
    }


    void SetupShader() override
    {
        _ShaderPipeline = CreateUnique<MeshShaderPipeline>(
            LOCAL_SHADERS_DIRECTORY  + "shader_multithread.mesh",
            GLOBAL_SHADERS_DIRECTORY + "animation\\shader.fs");
        model = convertModel(*_Model);
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

        if (m_RenderConfig.useClassicPipeline)
        {
            if (m_RenderConfig.showMeshlets) {
                model.DrawMeshletsSeparately(*m_ClassicShaderPipeline, *m_Camera);
            }
            else {
                model.Draw(*m_ClassicShaderPipeline, *m_Camera);
            }
        }
        else {
            model.DrawWithMeshShader(*_ShaderPipeline, *m_Camera);
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
        ImGui::SliderFloat("x", &model.Transform.position.x, -3.0f, 3.0f);
        ImGui::SliderFloat("y", &model.Transform.position.y, -3.0f, 3.0f);
        ImGui::SliderFloat("z", &model.Transform.position.z, -3.0f, 3.0f);

        ImGui::Text(u8"Ориентация");
        ImGui::SliderFloat("rx", &model.Transform.rotation.x, -180.0f, 180.0f);
        ImGui::SliderFloat("ry", &model.Transform.rotation.y, -180.0f, 180.0f);
        ImGui::SliderFloat("rz", &model.Transform.rotation.z, -180.0f, 180.0f);
        ImGui::End();

        ImGui::Begin(u8"Рендер");
        ImGui::Checkbox(u8"Использовать классический конвейер", &m_RenderConfig.useClassicPipeline);
        ImGui::Checkbox(u8"Показывать кластеры", &m_RenderConfig.showMeshlets);
        ImGui::Checkbox(u8"Каркас", &m_RenderConfig.wireframe);
        ImGui::Checkbox(u8"Фильтровать задние грани", &m_RenderConfig.backfaceCulling);
        ImGui::End();
    }
};

}