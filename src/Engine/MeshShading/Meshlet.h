#pragma once
#include "../EngineCore/Animation/Vertex.h"

namespace FQW {

struct Meshlet 
{
    // ClassID has 32 bytes because 16 or 8 bytes will crash convension of 16 bytes alignment
    // for OpenGL meshlet. Ofcourse we can use 'alignas(16)' directive, but this is not 
    // exists in GLSL.
    enum ClassID : uint32_t 
    {
        Alpha = 0,
        Betta = 1,
        Omega = 2
    };

    ClassID classID = Omega;
    vec4 cone;
    uint32_t vertices[64];
    uint8_t indices[126*3];
    uint8_t triangleCount = 0;
    uint8_t vertexCount = 0;
};


void CalculateMeshletCone(const vector<Vertex>& vertexBuffer, Meshlet& outMeshlet);
float CalculateConeAngle(const vec3 trisNormals[], size_t trisCount, const vec3& avgNormal);

} // namespace FQW::MeshShaders
