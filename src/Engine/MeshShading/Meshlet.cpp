#include "Meshlet.h"

namespace FQW {

void CalculateMeshletCone(const vector<Vertex>& vertexBuffer, Meshlet& outMeshlet)
{
    // TEMP:
    if (outMeshlet.triangleCount == 0)
        return;
    //assert(outMeshlet.triangleCount > 0);
    //assert(outMeshlet.vertexCount > 0);

    vec3 normals[126] = { vec3(0.0f ,0.0f, 0.0f) };

    for (size_t i = 0; i < outMeshlet.triangleCount; i++)
    {
        uint8_t a = outMeshlet.indices[i * 3 + 0];
        uint8_t b = outMeshlet.indices[i * 3 + 1];
        uint8_t c = outMeshlet.indices[i * 3 + 2];

        const Vertex& va = vertexBuffer[outMeshlet.vertices[a]];
        const Vertex& vb = vertexBuffer[outMeshlet.vertices[b]];
        const Vertex& vc = vertexBuffer[outMeshlet.vertices[c]];

        vec3 v1 = vb.position - va.position;
        vec3 v2 = vc.position - va.position;

        vec3 n = glm::cross(v1, v2);
        float l = glm::length(n);
        if (l == 0.0f)
            l = 1.0f;

        normals[i] = n / l;
    }

    vec3 avgNormal = { 0.0f, 0.0f, 0.0f };

    for (size_t i = 0; i < outMeshlet.triangleCount; i++)
    {
        avgNormal += normals[i];
    }

    if (glm::length(avgNormal) == 0.0f)
    {
        // We can have meshlet with cube-like form.
        // We can not cull such meshlets anyway, but must handle such case.
        avgNormal = vec3(1.0f, 0.0f, 0.0f);
    }
    else
        avgNormal = glm::normalize(avgNormal);

    assert(glm::length(avgNormal) > 0);

    float coneAngle = CalculateConeAngle(normals, outMeshlet.triangleCount, avgNormal);
    outMeshlet.cone = vec4(avgNormal, coneAngle);
}


float CalculateConeAngle(const vec3 trisNormals[], size_t trisCount, const vec3& avgNormal)
{
    float mindp = 1.0f;

    for (size_t i = 0; i < trisCount; i++)
    {
        float dp = glm::dot(trisNormals[i], avgNormal);

        mindp = std::min(mindp, dp);
    }

    return mindp;
}

float CalculateConeAngle_(const vec3 trisNormals[], size_t trisCount, const vec3& avgNormal)
{
    float angle = 1.0f;

    for (size_t i = 0; i < trisCount; i++)
    {
        float angleTemp = acos(glm::dot(avgNormal, trisNormals[i]));
        angle = std::max(angle, angleTemp);
    }

    return angle;
}


}