#pragma once
#include "../Main/App_Main.h"


namespace FQW::TestMeshShaders {

using FQW::NV_MeshShaders::Meshlet;
using glm::vec4;
using glm::mat4;

struct Vertex_
{
    vec4 position;
    vec4 normal;
    vec4 boneIds;
    vec4 boneWeights;
};

vector<Vertex_> convertMeshVertices(const Mesh& mesh)
{
    vector<Vertex_> result;

    for (const auto& v : mesh.GetVertexBuffer())
    {
        Vertex_ v_ =
        {
            vec4(v.position, 1.0f),
            vec4(v.normal, 0.0f),
            v.boneIds,
            v.boneWeights
        };

        result.push_back(v_);
    }

    return result;
}

struct Mesh_ 
{
    vector<Vertex_>  vertices;
    vector<uint32_t> indices;
    vector<Meshlet>  meshlets;
    GLuint vertex_ssbo;
    GLuint index_ssbo;
    GLuint meshlets_ssbo;

    Mesh_(vector<Vertex_> vertices, vector<uint32_t> indices, vector<Meshlet> meshlets)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->meshlets = meshlets;

        glcheck(glGenBuffers(1, &vertex_ssbo));
        glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_ssbo));
        glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vertex_) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW));
        glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

        glcheck(glGenBuffers(1, &index_ssbo));
        glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, index_ssbo));
        glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * this->indices.size(), this->indices.data(), GL_STATIC_DRAW));
        glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

        glcheck(glGenBuffers(1, &meshlets_ssbo));
        glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, meshlets_ssbo));
        glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Meshlet) * this->meshlets.size(), this->meshlets.data(), GL_STATIC_DRAW));
        glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
    }

    void Draw() const
    {
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertex_ssbo));
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, index_ssbo));
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, meshlets_ssbo));

        GLuint workgroupCount = (this->indices.size() / 3) / 32;
        glcheck(glDrawMeshTasksNV(0, workgroupCount));
    }
};

struct Model_ : public IDrawable 
{
    vector<Mesh_> meshes;

    void DrawWithMeshShader(ShaderPipeline& shader, ICamera& camera) override
    {
        mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix();

        shader.Use();
        shader.SetMatrix4fv(ShaderPipeline::ShaderType::Mesh, "u_mvp", mvp);

        for (const auto& mesh : meshes)
        {   mesh.Draw();  }
    }

    void Draw(ShaderPipeline& shader, ICamera& camera) override { FQW_WARN("Draw is not overriden"); }
};

Model_ convertModel(const Model& model)
{
    Model_ result;

    for (const auto& mesh : model.GetMeshes())
    {
        vector<Vertex_> vertices = convertMeshVertices(*mesh);
        vector<uint32_t> indices = mesh->GetIndexBuffer();

        auto meshletBuilder = FQW::NV_MeshShaders::NaiveMeshletBuilder();
        meshletBuilder.BuildForSingleMesh(*mesh);
        vector<Meshlet> meshlets = meshletBuilder.GetMeshlets();

        Mesh_ converted(vertices, indices, meshlets);
        result.meshes.push_back(converted);
    }

    return result;
}


struct App_TestMeshShader : public Main::App_Main
{
    const string LOCAL_SHADERS_DIRECTORY = s_SolutionDirectory + "src\\Demo\\Apps\\TestMeshShaders\\res\\";

    Model_ model;


    App_TestMeshShader()
        : App_Main() 
    { 
        m_Name = "Test Mesh Shader";
    }


    void SetupShader() override
    {
        _ShaderPipeline = CreateUnique<FQW::NV_MeshShaders::MeshShaderPipeline>(
            LOCAL_SHADERS_DIRECTORY + "v1\\shader.mesh.glsl",
            LOCAL_SHADERS_DIRECTORY + "basic.frag.glsl");
        model = convertModel(*_Model);
    }


    void Render() override
    {
        model.DrawWithMeshShader(*_ShaderPipeline, *m_Camera);
    }
};

}