#pragma once
#include "../EngineCore/Animation/Vertex.h"

namespace FQW {

struct Meshlet
{
    vec4 cone;
    uint32_t vertices[64];       // vertex indices in mesh vertexbuffer
    uint8_t indices[126*3];      // vertex indices in meshlet's vertexbuffer (Meshlet.vertices)
    uint8_t triangleCount = 0;
    uint8_t vertexCount = 0;
};

void CalculateMeshletCone(const vector<Vertex>& vertexBuffer, Meshlet& outMeshlet);
float CalculateConeAngle(const vec3 trisNormals[], size_t trisCount, const vec3& avgNormal);


} // namespace FQW::MeshShaders
