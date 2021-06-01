#pragma once
#include "../Scene/ICamera.h"
#include "Shader.h"

namespace FQW {

class IDrawable 
{
public:
    virtual void Draw(Shader& shader, ICamera& camera) = 0;

    virtual void DrawWithMeshShader(Shader& shader, ICamera& camera)
    {
        FQW_WARN("DrawWithMeshShader virtual method is not overriden and empty.");
    }
};

}