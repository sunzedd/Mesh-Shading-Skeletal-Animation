#pragma once
#include "../Animation/Mesh.h"

namespace FQW::NV_MeshShaders {

struct Meshlet
{
    uint32_t vertices[64];  // vertex indices in mesh vertexbuffer
    uint8_t indices[126];   // vertex indices in meshlet's vertexbuffer (Meshlet.vertices)
    uint8_t indexCount;
    uint8_t vertexCount;
};

} // namespace FQW::MeshShaders