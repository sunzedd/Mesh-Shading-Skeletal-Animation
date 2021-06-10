#pragma once
#include <Engine.h>

#include "Profiling/GPUPerformanceProfiler.h"
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
    Unique<ShaderPipeline> m_TaskMeshShaderPipeline;

    std::map<string, Ref<Model>> m_Models;
    Ref<Model> m_CurrentModel;
    float m_ModelScale = 1.0f;

    struct {
        int drawcallCount = 1;
        int pipelineId = 0;
        bool showMeshlets = false;
        bool wireframe = false;
        bool backfaceCulling = false;
    } m_RenderConfig;

    GPUPerformanceProfiler m_GPUProfiler;

public:
    App_MeshShaderDemo()
        : Application(1920, 1080, "Test Mesh Shader")
    {
        LoadModels();

        // Load shaders
        m_ClassicPipeline = CreateUnique<ClassicShaderPipeline>(
            SHADERS_DIRECTORY + "classic\\basic.vert",
            SHADERS_DIRECTORY + "classic\\basic.frag"
        );

        m_MeshShaderPipeline = CreateUnique<MeshShaderPipeline>(
            SHADERS_DIRECTORY + "turing_mesh\\basic.mesh",
            SHADERS_DIRECTORY + "turing_mesh\\basic.frag"
        ); 

        m_TaskMeshShaderPipeline = CreateUnique<TaskMeshShaderPipeline>(
            SHADERS_DIRECTORY + "turing_taskmesh\\basic.task",
            SHADERS_DIRECTORY + "turing_taskmesh\\basic.mesh",
            SHADERS_DIRECTORY + "turing_taskmesh\\basic.frag"
        );
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

        auto kittenModel = loader.LoadModel(MODELS_DIRECTORY + "kitten.obj");
        //auto spiderModelController = CreateRef<SpiderModelController>();
        //Script::Connect(spiderModel, spiderModelController);

        m_Models.insert({ "man", manModel });
        m_Models.insert({ "wolf", wolfModel });
        m_Models.insert({ "spider", spiderModel });
        m_Models.insert({ "kitten", kittenModel });

        RegisterScriptableEntity(manModel);
        RegisterScriptableEntity(wolfModel);
        RegisterScriptableEntity(spiderModel);
    }


    void Render() override
    {
        m_CurrentModel->Transform.scale = vec3(1.0f, 1.0f, 1.0f) * m_ModelScale;

        SetupRenderPipeline();

        // Actualy not only GPU time encountered, but CPU side number of commands is not so huge ...
        m_GPUProfiler.FrameBegin();

        if (m_RenderConfig.pipelineId == 0) 
        {
            for (int i = 0; i < m_RenderConfig.drawcallCount; i++) {
                m_CurrentModel->Draw(*m_ClassicPipeline, *m_Camera);
            }
        }
        else
        {
            if (m_RenderConfig.pipelineId == 1)
            {
                m_MeshShaderPipeline->SetBool(ShaderPipeline::ShaderStage::Fragment,
                                              "u_colorize_meshlet",
                                              m_RenderConfig.showMeshlets);

                for (int i = 0; i < m_RenderConfig.drawcallCount; i++) {
                    m_CurrentModel->Draw(*m_MeshShaderPipeline, *m_Camera);
                }
            }
            else if (m_RenderConfig.pipelineId == 2)
            {
                m_TaskMeshShaderPipeline->SetBool(ShaderPipeline::ShaderStage::Fragment,
                                                  "u_colorize_meshlet",
                                                  m_RenderConfig.showMeshlets);

                for (int i = 0; i < m_RenderConfig.drawcallCount; i++) {
                    m_CurrentModel->Draw(*m_TaskMeshShaderPipeline, *m_Camera);
                }
            }
        }

        m_GPUProfiler.FrameEnd();

        for (auto& scriptable : m_ScriptableEntities) {
            scriptable->OnDrawUI();
        }
    }

    void DrawUI() override
    {
        ImGui::Begin(u8"Производительность");
        ImGui::SetWindowFontScale(1.0);
        ImGui::Text(u8"Частота кадров: %.3f кадров/сек", 1.0f / m_DeltaTime * 1000.0f);
        ImGui::Text(u8"Время композиции кадра CPU + GPU: %.4f мс", m_DeltaTime);
        ImGui::Text(u8"Время композиции кадра GPU:       %.4f мс", m_GPUProfiler.GetFrameTime_ms());
        ImGui::Spacing();
        ImGui::SliderInt(u8"Вызовов отрисовки", &m_RenderConfig.drawcallCount, 1, 50);
        ImGui::End();

        ImGui::Begin(u8"Камера");
        ImGui::SetWindowFontScale(1.0);
        ImGui::Text(u8"Позиция [%.3f %.3f %.3f]", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
        ImGui::Text(u8"Ориентация: [%.3f %.3f, %.3f]", m_Camera->GetFront().x, m_Camera->GetFront().y, m_Camera->GetFront().z);
        ImGui::End();

        ImGui::Begin(u8"Модель");
        ImGui::SetWindowFontScale(1.0);

        ImGui::Text(u8"Позиция");
        ImGui::SliderFloat("x", &m_CurrentModel->Transform.position.x, -5.0f, 5.0f);
        ImGui::SliderFloat("y", &m_CurrentModel->Transform.position.y, -5.0f, 5.0f);
        ImGui::SliderFloat("z", &m_CurrentModel->Transform.position.z, -5.0f, 5.0f);

        ImGui::Text(u8"Ориентация");
        ImGui::SliderFloat("rx", &m_CurrentModel->Transform.rotation.x, -180.0f, 180.0f);
        ImGui::SliderFloat("ry", &m_CurrentModel->Transform.rotation.y, -180.0f, 180.0f);
        ImGui::SliderFloat("rz", &m_CurrentModel->Transform.rotation.z, -180.0f, 180.0f);

        ImGui::Text(u8"Масштаб");
        ImGui::SliderFloat("s", &m_ModelScale, 0.1f, 10.0f);
        ImGui::End();

        ImGui::Begin(u8"Рендер");
        ImGui::RadioButton("Classic", &m_RenderConfig.pipelineId, 0); ImGui::SameLine();
        ImGui::RadioButton("Mesh Shader", &m_RenderConfig.pipelineId, 1); ImGui::SameLine();
        ImGui::RadioButton("Task Mesh Shader", &m_RenderConfig.pipelineId, 2);
        ImGui::Separator();
        ImGui::Checkbox(u8"Только каркас", &m_RenderConfig.wireframe);
        ImGui::Checkbox(u8"Фильтровать задние грани", &m_RenderConfig.backfaceCulling);

        if (m_RenderConfig.pipelineId > 0) {
            ImGui::Checkbox(u8"Показывать кластеры", &m_RenderConfig.showMeshlets);
        }

        ImGui::End();
    }

    
    void SetupRenderPipeline()
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
            else {
                model->Deactivate();
            }
        }
    }

    void Update(float deltaTime) override
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
            This->Shutdown();
        }

        if (Input::IsKeyPressed(GLFW_KEY_1))
        {
            This->m_CurrentModel->Deactivate();
            This->m_CurrentModel = This->m_Models["wolf"];
            This->m_CurrentModel->Activate();
        }
        if (Input::IsKeyPressed(GLFW_KEY_2))
        {
            This->m_CurrentModel->Deactivate();
            This->m_CurrentModel = This->m_Models["man"];
            This->m_CurrentModel->Activate();
        }
        if (Input::IsKeyPressed(GLFW_KEY_3))
        {
            This->m_CurrentModel->Deactivate();
            This->m_CurrentModel = This->m_Models["spider"];
            This->m_CurrentModel->Activate();
        }
        if (Input::IsKeyPressed(GLFW_KEY_4))
        {
            This->m_CurrentModel->Deactivate();
            This->m_CurrentModel = This->m_Models["kitten"];
            This->m_CurrentModel->Activate();
        }
    }
};

}