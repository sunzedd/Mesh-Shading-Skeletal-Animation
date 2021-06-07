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
        string meshSource = ReadSource(m_MeshFilepath);
        string fragmentSource = ReadSource(m_FragmentFilepath);

        auto meshSource_ = static_cast <const GLchar*>(meshSource.c_str());
        auto fragmentSource_ = static_cast <const GLchar*>(fragmentSource.c_str());

        GLuint meshShaderId = Compile(meshSource_, ShaderType::Mesh, meshFilepath);
        GLuint fragmentShaderId = Compile(fragmentSource_, ShaderType::Fragment, fragmentFilepath);

        m_ShadersMap[ShaderType::Mesh] = meshShaderId;
        m_ShadersMap[ShaderType::Fragment] = fragmentShaderId;

        glcheck(glCreateProgramPipelines(1, &m_PipelineId));
        glcheck(glUseProgramStages(m_PipelineId, GL_MESH_SHADER_BIT_NV, meshShaderId));
        glcheck(glUseProgramStages(m_PipelineId, GL_FRAGMENT_SHADER_BIT, fragmentShaderId));
    }

private:
    string m_MeshFilepath;
    string m_FragmentFilepath;
};

}