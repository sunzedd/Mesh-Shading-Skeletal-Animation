#pragma once
#include <Engine.h>

#include "Scripts/ManModelController.h"
#include "Scripts/SpiderModelController.h"
#include "Scripts/WolfModelController.h"


namespace FQW::MeshShaderDemo {

class ApplicationController;

class App_MeshShaderDemo : public Application
{
public:
    const string SHADERS_DIRECTORY = s_SolutionDirectory + "src\\Demo\\Apps\\MeshShaderDemo\\Shaders\\";
    const string MODELS_DIRECTORY  = s_SolutionDirectory + "res\\meshes\\working\\";

    Unique<ShaderPipeline> m_ClassicPipeline;
    Unique<ShaderPipeline> m_MeshShaderPipeline;

    std::map<string, Ref<Model>> m_Models;
    Ref<Model> m_CurrentModel;

    struct {
        bool useClassicPipeline = false;
        bool showMeshlets = false;
        bool wireframe = false;
        bool backfaceCulling = false;
    } m_RenderConfig;


public:
    App_MeshShaderDemo()
        : Application(1920, 1080, "Test Mesh Shader")
    {
        LoadModels();

        // Load shaders
        m_ClassicPipeline = CreateUnique<ClassicShaderPipeline>(SHADERS_DIRECTORY + "basic.vert",
                                                                SHADERS_DIRECTORY + "classic.frag");

        m_MeshShaderPipeline = CreateUnique<MeshShaderPipeline>(SHADERS_DIRECTORY + "basic.mesh",
                                                                SHADERS_DIRECTORY + "turing.frag");    
    }


    void LoadModels()
    {
        MeshOptimizer meshOptimizer(true, true, true);
        auto loader = ModelLoader(meshOptimizer);
        
        auto manModel = loader.LoadModel(MODELS_DIRECTORY + "man.dae");
        auto manModelController = CreateRef<ManModelController>();
        Script::Connect(manModel, manModelController);

        auto wolfModel = loader.LoadModel(MODELS_DIRECTORY + "wolf.fbx");
        auto wolfModelController = CreateRef<WolfModelController>();
        Script::Connect(wolfModel, wolfModelController);

        meshOptimizer.doOptimization = false;

        auto spiderModel = loader.LoadModel(MODELS_DIRECTORY + "spider.fbx");
        auto spiderModelController = CreateRef<SpiderModelController>();
        Script::Connect(spiderModel, spiderModelController);

        m_Models.insert(std::make_pair("man", manModel));
        m_Models.insert(std::make_pair("wolf", wolfModel));
        m_Models.insert(std::make_pair("spider", spiderModel));

        RegisterScriptableEntity(manModel);
        RegisterScriptableEntity(wolfModel);
        RegisterScriptableEntity(spiderModel);
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
        else {
            glDisable(GL_CULL_FACE);
        }

        if (m_RenderConfig.useClassicPipeline) {
            m_CurrentModel->Draw(*m_ClassicPipeline, *m_Camera);
        }
        else {
            if (m_RenderConfig.showMeshlets) {
                m_MeshShaderPipeline->SetBool(ShaderPipeline::ShaderStage::Fragment, "u_colorize_meshlet", true);
            } 
            else {
                m_MeshShaderPipeline->SetBool(ShaderPipeline::ShaderStage::Fragment, "u_colorize_meshlet", false);
            }

            m_CurrentModel->Draw(*m_MeshShaderPipeline, *m_Camera);
        }

        for (auto& scriptable : m_ScriptableEntities) {
            scriptable->OnDrawUI();
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
        ImGui::SliderFloat("x", &m_CurrentModel->Transform.position.x, -3.0f, 3.0f);
        ImGui::SliderFloat("y", &m_CurrentModel->Transform.position.y, -3.0f, 3.0f);
        ImGui::SliderFloat("z", &m_CurrentModel->Transform.position.z, -3.0f, 3.0f);

        ImGui::Text(u8"Ориентация");
        ImGui::SliderFloat("rx", &m_CurrentModel->Transform.rotation.x, -180.0f, 180.0f);
        ImGui::SliderFloat("ry", &m_CurrentModel->Transform.rotation.y, -180.0f, 180.0f);
        ImGui::SliderFloat("rz", &m_CurrentModel->Transform.rotation.z, -180.0f, 180.0f);

        ImGui::Text(u8"Масштаб");
        ImGui::SliderFloat("s", &m_CurrentModel->Transform.scale.x, 0.1f, 10.0f);
        ImGui::SliderFloat("s", &m_CurrentModel->Transform.scale.y, 0.1f, 10.0f);
        ImGui::SliderFloat("s", &m_CurrentModel->Transform.scale.z, 0.1f, 10.0f);
        ImGui::End();

        ImGui::Begin(u8"Рендер");
        ImGui::Checkbox(u8"Использовать классический конвейер", &m_RenderConfig.useClassicPipeline);
        if (!m_RenderConfig.useClassicPipeline) {
            ImGui::Checkbox(u8"Показывать кластеры", &m_RenderConfig.showMeshlets);
        }
        ImGui::Checkbox(u8"Каркас", &m_RenderConfig.wireframe);
        ImGui::Checkbox(u8"Фильтровать задние грани", &m_RenderConfig.backfaceCulling);
        ImGui::End();
    }


    void Init()
    {
        auto applicationScript = CreateRef<ApplicationController>();
        Script::Connect(shared_from_this(), std::static_pointer_cast<Script>(applicationScript));
        RegisterUpdatableEntity(std::static_pointer_cast<Script>(applicationScript));
    }
};


class ApplicationController : public Script
{
private:
    Ref<App_MeshShaderDemo> This;

public:
    void FirstSetup() override
    {
        This = std::static_pointer_cast<App_MeshShaderDemo>(m_Entity);
    }

    void Start() override
    {
        for (auto& [name, model] : This->m_Models)
        {
            if (name == "spider")
            {
                model->Activate();
                This->m_CurrentModel = model;
            }
            else
            {
                model->Deactivate();
            }
        }
    }

    void Update(float deltaTime) override
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
            This->Shutdown();
        }

        if (Input::IsKeyPressed(GLFW_KEY_1)) {
            This->m_CurrentModel->Deactivate();
            This->m_CurrentModel = This->m_Models["spider"];
            This->m_CurrentModel->Activate();
        }
        if (Input::IsKeyPressed(GLFW_KEY_2)) {
            This->m_CurrentModel->Deactivate();
            This->m_CurrentModel = This->m_Models["man"];
            This->m_CurrentModel->Activate();
        }
        if (Input::IsKeyPressed(GLFW_KEY_3)) {
            This->m_CurrentModel->Deactivate();
            This->m_CurrentModel = This->m_Models["wolf"];
            This->m_CurrentModel->Activate();
        }
    }
};

}