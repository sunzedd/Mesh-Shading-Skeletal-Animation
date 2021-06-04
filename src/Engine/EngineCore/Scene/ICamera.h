#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Math.h"

#include "Script.h"


namespace FQW {

class ICamera : public ScriptableEntity
{
public:
    virtual const mat4& GetViewMatrix() const = 0;
    virtual const mat4& GetProjectionMatrix() const = 0;
};


} // namespace FQW