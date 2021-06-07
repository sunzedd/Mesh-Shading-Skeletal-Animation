#include "Transform.h"

namespace FQW {

const mat4& Transform::GetModelMatrix()
{
    CalculateModelMatrix();
    return m_ModelMatrix;
}

void Transform::CalculateModelMatrix()
{
    mat4 s = glm::identity<mat4>();
    mat4 rx = glm::identity<mat4>();
    mat4 ry = glm::identity<mat4>();
    mat4 rz = glm::identity<mat4>();
    mat4 r = glm::identity<mat4>();
    mat4 t = glm::identity<mat4>();

    s = glm::scale(s, scale);
    rx = glm::rotate(rx, glm::radians(rotation.x), vec3(1, 0, 0));
    ry = glm::rotate(ry, glm::radians(rotation.y), vec3(0, 1, 0));
    rz = glm::rotate(rz, glm::radians(rotation.z), vec3(0, 0, 1));
    r = rx * ry * rz;
    t = glm::translate(t, position);

    m_ModelMatrix = t * r * s;
}

} // namespace FQW