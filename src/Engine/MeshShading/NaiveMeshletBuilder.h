#pragma once
#include "IMeshletBuilder.h"


namespace FQW {

class NaiveMeshletBuilder final : public IMeshletBuilder
{
public:
    vector<Meshlet> Build(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer) override
    {
        vector<Meshlet> resultMeshlets;

        Meshlet meshlet = {};

        std::vector<uint32_t> meshletVertices(vertexBuffer.size(), 0xffffffff); // 0xffffffff: This vertex is not used in current meshlet

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
                resultMeshlets.push_back(meshlet);
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
            resultMeshlets.push_back(meshlet);
        }

        while (resultMeshlets.size() % 32 != 0)
        {
            resultMeshlets.push_back(Meshlet{});
        }

        for (auto& meshlet : resultMeshlets)
        {
            CalculateMeshletCone(vertexBuffer, meshlet);
        }

        return resultMeshlets;
    }
};

}