#pragma once
#include "../../meshoptimizer/src/meshoptimizer.h"
#include <Engine.h>

namespace FQW::TestMeshShaders2 {

struct TVertex
{
    vec3 position;
};


struct TMesh
{
    vector<TVertex> vertices;
    vector<GLuint> indices;
    GLuint vao, vbo, ebo;
    
    void GenVao()
    {
        glcheck(glGenVertexArrays(1, &vao));
        glcheck(glGenBuffers(1, &vbo));
        glcheck(glGenBuffers(1, &ebo));

        glcheck(glBindVertexArray(vao));

        glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        glcheck(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], GL_STATIC_DRAW));

        glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
        glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW));

        glcheck(glEnableVertexAttribArray(0));
        glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));
        glcheck(glBindVertexArray(0));
    }

    void Draw(ShaderPipeline& pipeline, const ICamera& cam)
    {
        pipeline.Use();
        pipeline.SetMatrix4fv(ShaderPipeline::ShaderType::Vertex, "u_mvp", cam.GetProjectionMatrix() * cam.GetViewMatrix());
        pipeline.SetVec3(ShaderPipeline::ShaderType::Vertex, "u_color", vec3(1.0, 1.0, 1.0));

        glcheck(glBindVertexArray(vao));
        glcheck(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0));
        glcheck(glBindVertexArray(0));
    }
};



Ref<TMesh> ConvertMesh(Ref<Mesh> mesh)
{
    const vector<Vertex>& vertices = mesh->GetVertexBuffer();
    const vector<uint32_t>& indices = mesh->GetIndexBuffer();

    Ref<TMesh> result = CreateRef<TMesh>();
    result->vertices.resize(vertices.size());
    result->indices = indices;

    for (int i = 0; i < vertices.size(); i++)
    {
        result->vertices[i] = TVertex{ vertices[i].position };
    }

    return result;
}


void OptimizeTMesh(TMesh& mesh)
{
    size_t indexCount = mesh.indices.size();
    vector<uint32_t> remap(indexCount);
    size_t vertexCount = meshopt_generateVertexRemap(&remap[0], &mesh.indices[0], indexCount, &mesh.vertices[0], mesh.vertices.size(), sizeof(TVertex));

    vector<TVertex> vertices(vertexCount);
    vector<uint32_t> indices(indexCount);

    meshopt_remapIndexBuffer(&indices[0], &mesh.indices[0], indexCount, &remap[0]);
    meshopt_remapVertexBuffer(&vertices[0], &mesh.vertices[0], indexCount, sizeof(TVertex), &remap[0]);

    FQW_TRACE("[OptimizeTMesh] before: vertices/indices size = {}/{}", mesh.vertices.size(), mesh.indices.size());
    FQW_TRACE("[OptimizeTMesh] after:  vertices/indices size = {}/{}", vertices.size(), indices.size());

    mesh.vertices = vertices;
    mesh.indices = indices;
}

vector<meshopt_Meshlet> BuildMeshlets(TMesh& mesh)
{
    const size_t max_vertices = 64;
    const size_t max_triangles = 124;
    const float cone_weight = 0.0f;

    size_t max_meshlets = meshopt_buildMeshletsBound(mesh.indices.size(), max_vertices, max_triangles);
    std::vector<meshopt_Meshlet> meshlets(max_meshlets);
    std::vector<unsigned int> meshlet_vertices(max_meshlets * max_vertices);
    std::vector<unsigned char> meshlet_triangles(max_meshlets * max_triangles * 3);

    size_t meshlet_count = meshopt_buildMeshlets(
        meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), mesh.indices.data(),
        mesh.indices.size(), &mesh.vertices[0].position.x, mesh.vertices.size(), sizeof(Vertex), max_vertices, max_triangles, cone_weight);
    
    FQW_TRACE("[Build Meshlets]: meshlets.size() = {}", meshlets.size());

    return meshlets;
}



}