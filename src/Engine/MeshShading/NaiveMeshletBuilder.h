#pragma once
#include "../Animation/Mesh.h"
#include "Meshlet.h"


namespace FQW::NV_MeshShaders {

// Class for meshlet building from the mesh in the naive maner
class NaiveMeshletBuilder
{
public:
    NaiveMeshletBuilder()
    {
    }

    void BuildForSingleMesh(const Mesh& mesh)
    {
        m_Meshlets.clear();
        Build(mesh);
    }

    void BuildForMultipleMeshes(const std::vector<Ref<Mesh>>& meshes)
    {
        m_Meshlets.clear();
        for (const auto& mesh : meshes) {
            Build(*mesh);
        }
    }

    vector<Meshlet>& GetMeshlets() { return m_Meshlets; }

private:
    void Build(const Mesh& mesh)
    {
        const std::vector<Vertex>& vertexBuffer = mesh.GetVertexBuffer();
        const std::vector<uint32_t>& indexBuffer = mesh.GetIndexBuffer();

        Meshlet meshlet = {};

        std::vector<uint8_t> meshletVertices(vertexBuffer.size(), 0xff); // 0xff = -1: We do not use this vertex in meshlet

        for (size_t i = 0; i < indexBuffer.size(); i += 3)
        {
            uint32_t a = indexBuffer[i + 0];    /* vertex indices in mesh vertexbuffer */
            uint32_t b = indexBuffer[i + 1];
            uint32_t c = indexBuffer[i + 2];
             
            uint8_t& av = meshletVertices[a];  /* vertex indices in meshlet's local vertexbuffer */
            uint8_t& bv = meshletVertices[b];
            uint8_t& cv = meshletVertices[c];

            if (meshlet.vertexCount + (av == 0xff) + (bv == 0xff) + (cv == 0xff) > 64 || 
                meshlet.indexCount + 1 > 126)
            {
                /* we exceed max vertex count or triangle count, so get a brand new meshlet */
                m_Meshlets.push_back(meshlet);
                meshlet = {};
                memset(meshletVertices.data(), 0xff, meshletVertices.size());
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

            meshlet.indices[meshlet.indexCount++] = av;
            meshlet.indices[meshlet.indexCount++] = bv;
            meshlet.indices[meshlet.indexCount++] = cv;
        }

        if (meshlet.indexCount > 0) // flush the last one
        {
            m_Meshlets.push_back(meshlet);
        }
    }

private:
    std::vector<Meshlet> m_Meshlets;
};


} // namespace FQW::MeshShaders