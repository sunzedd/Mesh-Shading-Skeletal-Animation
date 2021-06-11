#pragma once
#include "../EngineCore/Graphics/ShaderPipeline.h"

namespace FQW {

class MeshShaderPipeline : public ShaderPipeline
{
public:
    MeshShaderPipeline(string meshFilepath, string fragmentFilepath)
        :
        m_MeshFilepath(meshFilepath),
        m_FragmentFilepath(fragmentFilepath)
    {
        m_Type = ShaderPipeline::Type::TuringRTX_Mesh;

        string meshSource = ReadSource(m_MeshFilepath);
        string fragmentSource = ReadSource(m_FragmentFilepath);

        auto meshSource_ = static_cast <const GLchar*>(meshSource.c_str());
        auto fragmentSource_ = static_cast <const GLchar*>(fragmentSource.c_str());

        GLuint meshShaderId = Compile(meshSource_, ShaderStage::Mesh, m_MeshFilepath);
        GLuint fragmentShaderId = Compile(fragmentSource_, ShaderStage::Fragment, m_FragmentFilepath);

        m_ShadersMap[ShaderStage::Mesh] = meshShaderId;
        m_ShadersMap[ShaderStage::Fragment] = fragmentShaderId;

        glcheck(glCreateProgramPipelines(1, &m_PipelineId));
        glcheck(glUseProgramStages(m_PipelineId, GL_MESH_SHADER_BIT_NV, meshShaderId));
        glcheck(glUseProgramStages(m_PipelineId, GL_FRAGMENT_SHADER_BIT, fragmentShaderId));

        glDeleteShader(meshShaderId);
        glDeleteShader(fragmentShaderId);
    }

private:
    string m_MeshFilepath;
    string m_FragmentFilepath;
};

}