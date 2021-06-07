#pragma once
#include "../Core/Math.h"

namespace FQW {

class Transform
{
public:
    vec3 position = { 0, 0, 0 };
    vec3 rotation = { 0, 0, 0 };
    vec3 scale    = { 1, 1, 1 };

    const mat4& GetModelMatrix();

private:
    void CalculateModelMatrix();

private:
    mat4 m_ModelMatrix = glm::identity<mat4>();
};

} // namespace FQW