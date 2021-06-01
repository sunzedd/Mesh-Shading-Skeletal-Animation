#include "ClassicShaderPipeline.h"

namespace FQW {

ClassicShaderPipeline::ClassicShaderPipeline(string vertexFilepath, string fragmentFilepath)
    :
    m_VertexFilepath(vertexFilepath),
    m_FragmentFilepath(fragmentFilepath)
{
    string vertexSource = ReadSource(m_VertexFilepath);
    string fragmentSource = ReadSource(m_FragmentFilepath);

    auto vertexSource_ = static_cast <const GLchar*>(vertexSource.c_str());
    auto fragmentSource_ = static_cast <const GLchar*>(fragmentSource.c_str());

    GLuint vertexShaderId = Compile(vertexSource_, ShaderType::Vertex);
    GLuint fragmentShaderId = Compile(fragmentSource_, ShaderType::Fragment);

    m_ShadersMap[ShaderType::Vertex] = vertexShaderId;
    m_ShadersMap[ShaderType::Fragment] = fragmentShaderId;

    //glGenProgramPipelines(1, &m_PipelineId);
    glcheck( glCreateProgramPipelines(1, &m_PipelineId) );
    glcheck( glUseProgramStages(m_PipelineId, GL_VERTEX_SHADER_BIT, vertexShaderId) );
    glcheck( glUseProgramStages(m_PipelineId, GL_FRAGMENT_SHADER_BIT, fragmentShaderId) );
}

}