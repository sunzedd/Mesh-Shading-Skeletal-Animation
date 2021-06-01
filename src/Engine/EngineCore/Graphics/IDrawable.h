#pragma once
#include "../Scene/ICamera.h"
#include "ShaderPipeline.h"

namespace FQW {

class IDrawable 
{
public:
    virtual void Draw(ShaderPipeline& shader, ICamera& camera) = 0;
    virtual void DrawWithMeshShader(ShaderPipeline& shader, ICamera& camera)
    {
        FQW_WARN("DrawWithMeshShader virtual method is not overriden and empty.");
    }
};

}