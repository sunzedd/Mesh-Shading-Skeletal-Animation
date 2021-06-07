#pragma once
#include "../EngineCore/Animation/Mesh.h"

namespace FQW::NV_MeshShaders {

struct alignas(16) Meshlet
{
    uint32_t vertices[64];       // vertex indices in mesh vertexbuffer
    uint8_t indices[126*3];    // vertex indices in meshlet's vertexbuffer (Meshlet.vertices)
    uint8_t triangleCount = 0;
    uint8_t vertexCount = 0;
};

} // namespace FQW::MeshShaders
