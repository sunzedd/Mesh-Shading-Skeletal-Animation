#pragma once
#include <Engine.h>
#include "GraphicsData.h"
#include "../../meshoptimizer/src/meshoptimizer.h"


namespace FQW::TestMeshShaders2 {
using NV_MeshShaders::MeshShaderPipeline;

struct App_TestMeshShader : public Application
{
    const string MODEL_FILEPATH = s_SolutionDirectory + "res\\meshes\\working\\cubes.obj";
    const string RES = s_SolutionDirectory + "src\\Demo\\Apps\\TestMeshShaders2\\res";

    Unique<ShaderPipeline> m_ClassicPipeline;
    //Unique<MeshShaderPipeline> m_TuringPipeline;
    
    bool m_UseClassicPipeline = true;
    bool m_ShowMeshlets = false;

    Ref<TMesh> m_Mesh;


    App_TestMeshShader()
        : Application()
    {
        m_ClassicPipeline = CreateUnique<ClassicShaderPipeline>(RES + "\\basic.vert", RES + "\\basic.frag");
        Ref<Model> model = ModelLoader::LoadModel(MODEL_FILEPATH);
        Ref<Mesh> mesh = model->GetMeshes()[0];

        m_Mesh = ConvertMesh(mesh);
        //OptimizeTMesh(*m_Mesh);
        //vector<meshopt_Meshlet> meshlets = BuildMeshlets(*m_Mesh);
        m_Mesh->GenVao();
    }

    void DrawUI()
    {
        ImGui::Begin(u8"Тест Mesh Shaders");
        ImGui::Checkbox(u8"Использовать классический конвейер", &m_UseClassicPipeline);
        ImGui::Checkbox(u8"Показать кластеризацию сетки", &m_ShowMeshlets);
        ImGui::End();
    }

    void Render()
    {
        m_Mesh->Draw(*m_ClassicPipeline, *m_Camera);
    }
};


}