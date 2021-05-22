#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BehaviourScript.h"

namespace FQW {

class ICamera : public ScriptableEntity
{
public:
    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
};


} // namespace FQW