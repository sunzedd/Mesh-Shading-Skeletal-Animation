#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Math.h"

#include "Script.h"


namespace FQW {

class ICamera : public ScriptableEntity
{
public:
    virtual const vec3& GetPosition() const = 0;
    virtual const vec3& GetViewDirection() = 0;
    virtual const mat4& GetViewMatrix() = 0;
    virtual const mat4& GetProjectionMatrix() = 0;
    virtual const mat4& GetViewProjectionMatrix() = 0;
};


} // namespace FQW