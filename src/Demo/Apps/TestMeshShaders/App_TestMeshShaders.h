#pragma once
#include "../Main/App_Main.h"
#include "NewTypes.h"


namespace FQW::TestMeshShaders {

struct App_TestMeshShader : public Main::App_Main
{
    const string LOCAL_SHADERS_DIRECTORY = s_SolutionDirectory + "src\\Demo\\Apps\\TestMeshShaders\\res\\";
    Model_ model;
    Unique<ShaderPipeline> m_ClassicShaderPipeline;

    bool m_UseClassicPipeline = true;
    bool m_Wireframe = true;
    bool m_BackfaceCulling = false;
    bool m_DrawMeshletsSeparately = false;


    App_TestMeshShader()
        : App_Main() 
    { 
        m_Name = "Test Mesh Shader";

        m_ClassicShaderPipeline = CreateUnique<ClassicShaderPipeline>(
            s_SolutionDirectory + "res\\shaders\\animation\\shader.vs",
            s_SolutionDirectory + "res\\shaders\\animation\\shader.fs");
    }


    void SetupShader() override
    {
        _ShaderPipeline = CreateUnique<FQW::NV_MeshShaders::MeshShaderPipeline>(
            LOCAL_SHADERS_DIRECTORY + "v2\\shader.mesh",
            LOCAL_SHADERS_DIRECTORY + "basic.frag.glsl");
        model = convertModel(*_Model);
    }


    void Render() override
    {
        if (m_Wireframe) 
            glPolygonMode(GL_FRONT, GL_LINE);
        else
            glPolygonMode(GL_FRONT, GL_FILL);
        
        if (m_BackfaceCulling)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        if (m_UseClassicPipeline) 
        {
            if (m_DrawMeshletsSeparately) 
            {
                model.DrawMeshletsSeparately(*m_ClassicShaderPipeline, *m_Camera);
            }
            else
            {
                model.Draw(*m_ClassicShaderPipeline, *m_Camera);
            }
        }
        else
        {
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
        ImGui::SliderFloat("x", &model.transform.position.x, -3.0f, 3.0f);
        ImGui::SliderFloat("y", &model.transform.position.y, -3.0f, 3.0f);
        ImGui::SliderFloat("z", &model.transform.position.z, -3.0f, 3.0f);

        ImGui::Text(u8"Ориентация");
        ImGui::SliderFloat("rx", &model.transform.rotation.x, -180.0f, 180.0f);
        ImGui::SliderFloat("ry", &model.transform.rotation.y, -180.0f, 180.0f);
        ImGui::SliderFloat("rz", &model.transform.rotation.z, -180.0f, 180.0f);
        ImGui::End();

        ImGui::Begin(u8"Рендер");
        ImGui::Checkbox(u8"Использовать классический конвейер", &m_UseClassicPipeline);
        ImGui::Checkbox(u8"Отрисовать отдельные мешлеты классически", &m_DrawMeshletsSeparately);
        ImGui::Checkbox(u8"Каркас", &m_Wireframe);
        ImGui::Checkbox(u8"Фильтровать задние грани", &m_BackfaceCulling);
        ImGui::End();
    }
};

}