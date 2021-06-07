#pragma once
#include <Engine.h>
#include "../../meshoptimizer/src/meshoptimizer.h"


#define MAX_COLORS 10


namespace FQW::TestMeshShaders {

using FQW::NV_MeshShaders::Meshlet;
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

struct Vertex_
{
    vec3 position;
};

vector<Vertex_> convertMeshVertices(const Mesh& mesh)
{
    vector<Vertex_> result;

    for (const auto& v : mesh.GetVertexBuffer())
    {
        result.push_back(Vertex_{ v.position });
    }

    return result;
}

struct Mesh_
{
    vector<Vertex_>  vertices;
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


    Mesh_(vector<Vertex_> vertices, vector<uint32_t> indices, vector<Meshlet> meshlets)
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
            vector<Vertex_> meshlet_vertices{};
            for (int local_vi = 0; local_vi < meshlets[i].vertexCount; local_vi++)
            {
                uint32_t global_vi = meshlets[i].vertices[local_vi];
                Vertex_& v = this->vertices[global_vi];
                meshlet_vertices.push_back(v);
            }

            glcheck(glBindBuffer(GL_ARRAY_BUFFER, meshlet_vbo));
            glcheck(glBufferData(GL_ARRAY_BUFFER, meshlets[i].vertexCount * sizeof(Vertex_), &meshlet_vertices[0], GL_STATIC_DRAW));

            // EBO
            glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshlet_ebo));
            glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshlets[i].vertexCount * 3 * sizeof(uint8_t), &(meshlets[i].indices[0]), GL_STATIC_DRAW));

            // Vertex attribute layout
            glcheck(glEnableVertexAttribArray(0));
            glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_), (GLvoid*)0));
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
            glcheck(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex_), &(this->vertices[0]), GL_STATIC_DRAW));

            // EBO
            glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
            glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(uint32_t), &(this->indices[0]), GL_STATIC_DRAW));

            // Vertex attribute layout
            glcheck(glEnableVertexAttribArray(0));
            glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_), (GLvoid*)0));
            glcheck(glBindVertexArray(0));
        }

        // For mesh shader usage
        {
            glcheck(glGenBuffers(1, &vertex_ssbo));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_ssbo));
            glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(this->vertices[0]) * this->vertices.size(), &(this->vertices[0]), GL_STATIC_DRAW));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

            glcheck(glGenBuffers(1, &index_ssbo));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, index_ssbo));
            glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(this->indices[0]) * this->indices.size(), &(this->indices[0]), GL_STATIC_DRAW));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

            glcheck(glGenBuffers(1, &meshlets_ssbo));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, meshlets_ssbo));
            glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(this->meshlets[0]) * this->meshlets.size(), &(this->meshlets[0]), GL_STATIC_DRAW));
            glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
        }
    }

    void DrawMeshletsSeparately(ShaderPipeline& shader) const
    {
        for (int i = 0; i < meshlets_vao.size(); i++)
        {
            shader.SetVec3(ShaderPipeline::ShaderType::Vertex, "u_color", meshlets_colors[i]);
            glcheck(glBindVertexArray(meshlets_vao[i]));
            glcheck(glDrawElements(GL_TRIANGLES, this->meshlets[i].triangleCount * 3, GL_UNSIGNED_BYTE, (GLvoid*)0));
            glcheck(glBindVertexArray(0));
        }
    }

    void DrawWithMeshShader(ShaderPipeline& shader) const
    {
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertex_ssbo));
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, index_ssbo));
        glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, meshlets_ssbo));

        GLuint workgroupCount = this->meshlets.size();
        shader.SetInt(ShaderPipeline::ShaderType::Mesh, "u_meshlet_count", workgroupCount);
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
    struct
    {
        vec3 position = vec3(0, 0, 0);
        vec3 rotation = vec3(0, 0, 0);
        vec3 scale = vec3(1, 1, 1);
    } transform;

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
        mat4 S = mat4(1.0f);
        mat4 Rx = mat4(1.0f);
        mat4 Ry = mat4(1.0f);
        mat4 Rz = mat4(1.0f);
        mat4 R = mat4(1.0f);
        mat4 T = mat4(1.0f);

        mat4 M = mat4(1.0f);
        mat4 V = mat4(1.0f);
        mat4 P = mat4(1.0f);
        mat4 VP = mat4(1.0f);

        S = glm::scale(S, transform.scale);
        Rx = glm::rotate(Rx, glm::radians(transform.rotation.x), vec3(1, 0, 0));
        Ry = glm::rotate(Ry, glm::radians(transform.rotation.y), vec3(0, 1, 0));
        Rz = glm::rotate(Rz, glm::radians(transform.rotation.z), vec3(0, 0, 1));
        R = Rx * Ry * Rz;
        T = glm::translate(T, transform.position);

        M = T * R * S;
        V = camera.GetViewMatrix();
        P = camera.GetProjectionMatrix();
        VP = P * V;

        shader.Use();
        shader.SetMatrix4fv(type, "model_matrix", M);
        shader.SetMatrix4fv(type, "view_projection_matrix", VP);
        shader.SetMatrix4fv(type, "u_mvp", M * VP);
    }
};

void optimizeMesh(vector<Vertex_>& vertices, vector<uint32_t>& indices)
{
    size_t indexCount = indices.size();
    vector<uint32_t> remap(indexCount);
    size_t vertexCount = meshopt_generateVertexRemap(&remap[0], &indices[0], indexCount, &vertices[0], vertices.size(), sizeof(Vertex_));

    vector<Vertex_> vertices_(vertexCount);
    vector<uint32_t> indices_(indexCount);

    meshopt_remapIndexBuffer(&indices_[0], &indices[0], indexCount, &remap[0]);
    meshopt_remapVertexBuffer(&vertices_[0], &vertices[0], indexCount, sizeof(Vertex_), &remap[0]);

    vertices = vertices_;
    indices = indices_;
}

vector<Meshlet> buildMeshlets(const vector<Vertex_>& vertexBuffer, const vector<uint32_t>& indexBuffer)
{
    vector<Meshlet> result;

    Meshlet meshlet = {};

    std::vector<uint8_t> meshletVertices(vertexBuffer.size(), 0xff); // 0xff: We do not use this vertex in meshlet

    for (size_t i = 0; i < indexBuffer.size(); i += 3)
    {
        uint32_t a = indexBuffer[i + 0];    /* vertex indices in mesh vertexbuffer */
        uint32_t b = indexBuffer[i + 1];
        uint32_t c = indexBuffer[i + 2];

        uint8_t& av = meshletVertices[a];  /* vertex indices in meshlet's local vertexbuffer */
        uint8_t& bv = meshletVertices[b];
        uint8_t& cv = meshletVertices[c];

        if (meshlet.vertexCount + (av == 0xff) + (bv == 0xff) + (cv == 0xff) > 64 ||
            meshlet.triangleCount >= 126)
        {
            /* we exceed max vertex count or triangle count, so get a brand new meshlet */
            result.push_back(meshlet);
            for (size_t j = 0; j < meshlet.vertexCount; ++j)
            {
                meshletVertices[meshlet.vertices[j]] = 0xff;
            }
            meshlet = {};
        }

        if (av == 0xff) /* if vertex 'a' is not stored in meshlet's local vertexbuffer yet, put it into meshlet */
        {
            av = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = a;
        }
        if (bv == 0xff)
        {
            bv = meshlet.vertexCount;
            meshlet.vertices[meshlet.vertexCount++] = b;
        }
        if (cv == 0xff)
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


vector<Meshlet> buildMeshlets_(const vector<Vertex_>& vertexBuffer, const vector<uint32_t>& indexBuffer)
{
    vector<Meshlet> resultMeshlets;

    Meshlet m{};
    uint32_t indexCount = 0;

    // ѕусть vi - индекс вершины в vertexBuffer, котора€ должна попасть в мешлет m, “огда
    // localVertexIndices[vi] = vi_local - индекс, под которым эта вершина занесена в локальный буфер вершин мешлета.
    // значение -1 означает, что эта вершина еще не находитс€ в мешлете.
    vector<int> localVertexIndices(vertexBuffer.size(), -1);

    for (size_t i = 0; i < indexBuffer.size(); i += 3)
    {
        // “реугольник
        uint32_t idx0 = indexBuffer[i + 0];
        uint32_t idx1 = indexBuffer[i + 1];
        uint32_t idx2 = indexBuffer[i + 2];

        if (localVertexIndices[idx0] == -1)
        {
            localVertexIndices[idx0] = m.vertexCount;
            m.vertices[m.vertexCount] = localVertexIndices[idx0];
            m.indices[indexCount] = m.vertexCount;
            m.vertexCount++;
            indexCount++;
        }
        else
        {
            m.indices[indexCount] = localVertexIndices[idx0];
            indexCount++;
        }

        if (localVertexIndices[idx1] == -1)
        {
            localVertexIndices[idx1] = m.vertexCount;
            m.vertices[m.vertexCount] = localVertexIndices[idx1];
            m.indices[indexCount] = m.vertexCount;
            m.vertexCount++;
            indexCount++;
        }
        else
        {
            m.indices[indexCount] = localVertexIndices[idx1];
            indexCount++;
        }

        if (localVertexIndices[idx2] == -1)
        {
            localVertexIndices[idx2] = m.vertexCount;
            m.vertices[m.vertexCount] = localVertexIndices[idx2];
            m.indices[indexCount] = m.vertexCount;
            m.vertexCount++;
            indexCount++;
        }
        else
        {
            m.indices[indexCount] = localVertexIndices[idx2];
            indexCount++;
        }
    }

    m.triangleCount = indexCount / 3;
    resultMeshlets.push_back(m);

    return resultMeshlets;
}




Model_ convertModel(const Model& model)
{
    Model_ result;

    for (const auto& mesh : model.GetMeshes())
    {
        vector<Vertex_> vertices = convertMeshVertices(*mesh);
        vector<uint32_t> indices = mesh->GetIndexBuffer();
        //optimizeMesh(vertices, indices);
        vector<Meshlet> meshlets = buildMeshlets(vertices, indices);

        Mesh_ converted(vertices, indices, meshlets);
        result.meshes.push_back(converted);
    }

    return result;
}

}