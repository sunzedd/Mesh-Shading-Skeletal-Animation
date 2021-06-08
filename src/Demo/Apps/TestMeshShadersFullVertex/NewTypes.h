#pragma once
#include <Engine.h>
#include "../../meshoptimizer/src/meshoptimizer.h"


#define MAX_COLORS 10


namespace FQW::TestMeshShaders {

using FQW::Meshlet;
using glm::vec4;
using glm::mat4;

static const vec3 colors[MAX_COLORS] = {
    vec3(1,0,0),
    vec3(0,1,0),
    vec3(0,0,1),
    vec3(1,1,0),
    vec3(1,0,1),
    vec3(0,1,1),
    vec3(1,0.5,0),
    vec3(0.5,1,0),
    vec3(0,0.5,1),
    vec3(1,1,1)
};


struct Mesh_
{
    vector<Vertex>  vertices;
    vector<uint32_t> indices;
    vector<Meshlet>  meshlets;
    vector<GLuint> meshlets_vao;
    vector<vec3> meshlets_colors;

    // For classic pipeline usage
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    // For mesh shader usage
    GLuint vertex_ssbo;
    GLuint index_ssbo;
    GLuint meshlets_ssbo;


    Mesh_(vector<Vertex> vertices, vector<uint32_t> indices, vector<Meshlet> meshlets)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->meshlets = meshlets;

        // For drawing meshlets separately 
        for (int i = 0; i < meshlets.size(); i++)
        {
            GLuint meshlet_vao, meshlet_vbo, meshlet_ebo;
            glcheck(glGenVertexArrays(1, &meshlet_vao));
            glcheck(glGenBuffers(1, &meshlet_vbo));
            glcheck(glGenBuffers(1, &meshlet_ebo));

            glcheck(glBindVertexArray(meshlet_vao));

            // VBO
            vector<Vertex> meshlet_vertices{};
            for (int local_vi = 0; local_vi < meshlets[i].vertexCount; local_vi++)
            {
                uint32_t global_vi = meshlets[i].vertices[local_vi];
                Vertex& v = this->vertices[global_vi];
                meshlet_vertices.push_back(v);
            }

            glcheck(glBindBuffer(GL_ARRAY_BUFFER, meshlet_vbo));
            glcheck(glBufferData(GL_ARRAY_BUFFER, meshlets[i].vertexCount * sizeof(Vertex), &meshlet_vertices[0], GL_STATIC_DRAW));

            // EBO
            glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshlet_ebo));
            glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshlets[i].triangleCount * 3 * sizeof(uint32_t), &(meshlets[i].indices[0]), GL_STATIC_DRAW));

            // Vertex attribute layout
            glcheck(glEnableVertexAttribArray(0));
            glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));
            glcheck(glBindVertexArray(0));

            meshlets_vao.push_back(meshlet_vao);
            meshlets_colors.push_back(colors[i % MAX_COLORS]);
        }

        // For default pipeline usage
        {
            // VAO, VBO, EBO generation
            glcheck(glGenVertexArrays(1, &vao));
            glcheck(glGenBuffers(1, &vbo));
            glcheck(glGenBuffers(1, &ebo));

            glcheck(glBindVertexArray(vao));

            // VBO
            glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
            glcheck(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &(this->vertices[0]), GL_STATIC_DRAW));

            // EBO
            glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
            glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(uint32_t), &(this->indices[0]), GL_STATIC_DRAW));

            // Vertex attribute layout
            glcheck(glEnableVertexAttribArray(0));
            glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));
            glcheck(glBindVertexArray(0));
        }

        // For mesh shader usage
        {
            glcheck(glGenBuffers(1, &vertex_ssbo));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_ssbo));
            glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(this->vertices[0]) * this->vertices.size(), &(this->vertices[0]), GL_STATIC_DRAW));
            glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, meshlets_ssbo));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

            glcheck(glGenBuffers(1, &meshlets_ssbo));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, meshlets_ssbo));
            glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(this->meshlets[0]) * this->meshlets.size(), &(this->meshlets[0]), GL_STATIC_DRAW));
            glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, meshlets_ssbo));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
        }
    }

    void DrawMeshletsSeparately(ShaderPipeline& shader) const
    {
        for (int i = 0; i < meshlets_vao.size(); i++)
        {
            shader.SetVec3(ShaderPipeline::ShaderType::Vertex, "u_color", meshlets_colors[i]);
            glcheck(glBindVertexArray(meshlets_vao[i]));
            glcheck(glDrawElements(GL_TRIANGLES, this->meshlets[i].triangleCount * 3, GL_UNSIGNED_INT, (GLvoid*)0));
            glcheck(glBindVertexArray(0));
        }
    }

    void DrawWithMeshShader(ShaderPipeline& shader) const
    {
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertex_ssbo));
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, meshlets_ssbo));
        GLuint workgroupCount = this->meshlets.size();
        glcheck(glDrawMeshTasksNV(0, workgroupCount));
    }

    void Draw() const
    {
        glcheck(glBindVertexArray(vao));
        glcheck(glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (GLvoid*)0));
        glcheck(glBindVertexArray(0));
    }
};

struct Model_ : public IDrawable
{
    vector<Mesh_> meshes;
    Transform Transform;

    void DrawMeshletsSeparately(ShaderPipeline& shader, ICamera& camera)
    {
        BindShaderUniforms(ShaderPipeline::ShaderType::Vertex, shader, camera);
        for (const auto& mesh : meshes)
        {
            mesh.DrawMeshletsSeparately(shader);
        }
    }

    void DrawWithMeshShader(ShaderPipeline& shader, ICamera& camera) override
    {
        BindShaderUniforms(ShaderPipeline::ShaderType::Mesh, shader, camera);
        for (const auto& mesh : meshes)
        {
            mesh.DrawWithMeshShader(shader);
        }
    }

    void Draw(ShaderPipeline& shader, ICamera& camera) override
    {
        BindShaderUniforms(ShaderPipeline::ShaderType::Vertex, shader, camera);
        for (const auto& mesh : meshes)
        {
            mesh.Draw();
        }
    }

    void BindShaderUniforms(ShaderPipeline::ShaderType type, ShaderPipeline& shader, ICamera& camera)
    {
        const mat4& m = Transform.GetModelMatrix();
        const mat4& vp = camera.GetViewProjectionMatrix();
        mat4 mvp = vp * m;

        shader.Use();
        shader.SetMatrix4fv(type, "u_MVP_matrix", mvp);
    }
};

void optimizeMesh(vector<Vertex>& vertices, vector<uint32_t>& indices, bool optimizeVertexCache, bool optimizeVertexFetch)
{
    size_t indexCount = indices.size();
    vector<uint32_t> remap(indexCount);
    size_t vertexCount = meshopt_generateVertexRemap(&remap[0], indices.data(), indexCount, &vertices[0], vertices.size(), sizeof(Vertex));

    vector<Vertex> vertices_(vertexCount);
    vector<uint32_t> indices_(indexCount);

    meshopt_remapIndexBuffer(&indices_[0], &indices[0], indexCount, &remap[0]);
    meshopt_remapVertexBuffer(&vertices_[0], &vertices[0], indexCount, sizeof(Vertex), &remap[0]);

    if (optimizeVertexCache)
    {
        meshopt_optimizeVertexCache(indices_.data(), indices_.data(), indexCount, vertexCount);
    }
    if (optimizeVertexFetch)
    {
        meshopt_optimizeVertexFetch(vertices_.data(), indices_.data(), indexCount, vertices_.data(), vertexCount, sizeof(Vertex));
    }

    vertices = vertices_;
    indices = indices_;
}

vector<Meshlet> buildMeshlets(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer)
{
    vector<Meshlet> result;

    Meshlet meshlet = {};

    std::vector<uint32_t> meshletVertices(vertexBuffer.size(), 0xffffffff); // 0xffffffff: We do not use this vertex in meshlet

    for (size_t i = 0; i < indexBuffer.size(); i += 3)
    {
        uint32_t a = indexBuffer[i + 0];    /* vertex indices in mesh vertexbuffer */
        uint32_t b = indexBuffer[i + 1];
        uint32_t c = indexBuffer[i + 2];

        uint32_t& av = meshletVertices[a];  /* vertex indices in meshlet's local vertexbuffer */
        uint32_t& bv = meshletVertices[b];
        uint32_t& cv = meshletVertices[c];

        if (meshlet.vertexCount + (av == 0xffffffff) + (bv == 0xffffffff) + (cv == 0xffffffff) > 64 ||
            meshlet.triangleCount >= 126)
        {
            /* we exceed max vertex count or triangle count, so get a brand new meshlet */
            result.push_back(meshlet);
            for (size_t j = 0; j < meshlet.vertexCount; ++j)
            {
                meshletVertices[meshlet.vertices[j]] = 0xffffffff;
            }
            meshlet = {};
        }

        if (av == 0xffffffff) /* if vertex 'a' is not stored in meshlet's local vertexbuffer yet, put it into meshlet */
        {
            av = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = a;
        }
        if (bv == 0xffffffff)
        {
            bv = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = b;
        }
        if (cv == 0xffffffff)
        {
            cv = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = c;
        }

        meshlet.indices[meshlet.triangleCount * 3 + 0] = av;
        meshlet.indices[meshlet.triangleCount * 3 + 1] = bv;
        meshlet.indices[meshlet.triangleCount * 3 + 2] = cv;
        meshlet.triangleCount++;
    }

    if (meshlet.triangleCount > 0) // flush the last one
    {
        result.push_back(meshlet);
    }

    return result;
}

vector<Meshlet> buildMeshletsForCubesScene(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer)
{
    vector<Meshlet> result;

    Meshlet meshlet = {};

    std::vector<uint32_t> meshletVertices(vertexBuffer.size(), 0xffffffff); // 0xffffffff: We do not use this vertex in meshlet

    for (size_t i = 0; i < indexBuffer.size(); i += 3)
    {
        uint32_t a = indexBuffer[i + 0];    /* vertex indices in mesh vertexbuffer */
        uint32_t b = indexBuffer[i + 1];
        uint32_t c = indexBuffer[i + 2];

        uint32_t& av = meshletVertices[a];  /* vertex indices in meshlet's local vertexbuffer */
        uint32_t& bv = meshletVertices[b];
        uint32_t& cv = meshletVertices[c];

        if (meshlet.vertexCount + (av == 0xffffffff) + (bv == 0xffffffff) + (cv == 0xffffffff) > 36 ||
            meshlet.triangleCount >= 126)
        {
            /* we exceed max vertex count or triangle count, so get a brand new meshlet */
            result.push_back(meshlet);
            for (size_t j = 0; j < meshlet.vertexCount; ++j)
            {
                meshletVertices[meshlet.vertices[j]] = 0xffffffff;
            }
            meshlet = {};
        }

        if (av == 0xffffffff) /* if vertex 'a' is not stored in meshlet's local vertexbuffer yet, put it into meshlet */
        {
            av = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = a;
        }
        if (bv == 0xffffffff)
        {
            bv = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = b;
        }
        if (cv == 0xffffffff)
        {
            cv = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = c;
        }

        meshlet.indices[meshlet.triangleCount * 3 + 0] = av;
        meshlet.indices[meshlet.triangleCount * 3 + 1] = bv;
        meshlet.indices[meshlet.triangleCount * 3 + 2] = cv;
        meshlet.triangleCount++;
    }

    if (meshlet.triangleCount > 0) // flush the last one
    {
        result.push_back(meshlet);
    }

    return result;
}



Model_ convertModel(const Model& model)
{
    Model_ result;

    for (const auto& mesh : model.GetMeshes())
    {
        vector<Vertex> vertices = mesh->GetVertexBuffer();
        vector<uint32_t> indices = mesh->GetIndexBuffer();
        
        // Using Zeux Meshoptimizer
        optimizeMesh(vertices, indices, true, true);
        
        vector<Meshlet> meshlets = buildMeshlets(vertices, indices);

        Mesh_ converted(vertices, indices, meshlets);
        result.meshes.push_back(converted);
    }

    return result;
}

}