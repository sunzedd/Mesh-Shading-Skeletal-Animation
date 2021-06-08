#pragma once
#include "../Scene/ICamera.h"
#include "ShaderPipeline.h"

namespace FQW {

class IDrawable 
{
public:
    virtual void Draw(ShaderPipeline& shaderPipeline, ICamera& camera) = 0;
};

}