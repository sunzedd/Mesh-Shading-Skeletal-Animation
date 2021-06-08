#pragma once
#include "../../meshoptimizer/src/meshoptimizer.h"

#include "Mesh.h"


namespace FQW {


class MeshOptimizer
{
public:
    bool doOptimization = false;
    bool optimizeVertexCache = false;
    bool optimizeVertexFetch = false;


    // TODO:
    // Bad design, rewrite it later
    MeshOptimizer()
        :
        doOptimization(false),
        optimizeVertexCache(false),
        optimizeVertexFetch(false)
    { }

    MeshOptimizer(bool doOptimization, bool optimizeVertexCache=false, bool optimizeVertexFetch=false)
        :
        doOptimization(doOptimization),
        optimizeVertexCache(optimizeVertexCache),
        optimizeVertexFetch(optimizeVertexFetch)
    { }


    void OptimizeMeshData(vector<Vertex>& vertices, vector<uint32_t>& indices)
    {
        if (!doOptimization) {
            return;
        }

        OptimizeMeshDataInternal(vertices, indices);
    }

private:
    void OptimizeMeshDataInternal(vector<Vertex>& vertices, vector<uint32_t>& indices)
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
};

}
