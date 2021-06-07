#pragma once
#include "../EngineCore/Animation/Mesh.h"

namespace FQW {

struct Meshlet
{
    uint32_t vertices[64];       // vertex indices in mesh vertexbuffer
    uint32_t indices[126*3];    // vertex indices in meshlet's vertexbuffer (Meshlet.vertices)
    uint32_t triangleCount = 0;
    uint32_t vertexCount = 0;
};

} // namespace FQW::MeshShaders
