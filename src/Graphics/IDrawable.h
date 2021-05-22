#pragma once
#include "../Scene/ICamera.h"
#include "Shader.h"

namespace FQW {

class IDrawable 
{
public:
    virtual void Draw(Shader& shader, ICamera& camera) = 0;
};

}