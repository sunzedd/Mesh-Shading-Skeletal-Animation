#pragma once
#include "ShaderPipeline.h"

namespace FQW {

class ClassicShaderPipeline : public ShaderPipeline
{
public:
    ClassicShaderPipeline(string vertexFilepath, string fragmentFilepath);

private:
    string m_VertexFilepath;
    string m_FragmentFilepath;
};

}