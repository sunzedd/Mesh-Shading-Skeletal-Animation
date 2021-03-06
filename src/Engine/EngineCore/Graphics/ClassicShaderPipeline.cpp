#include "ClassicShaderPipeline.h"

namespace FQW {

ClassicShaderPipeline::ClassicShaderPipeline(const string& vertexFilepath, const string& fragmentFilepath)
    :
    m_VertexFilepath(vertexFilepath),
    m_FragmentFilepath(fragmentFilepath)
{
    m_Type = ShaderPipeline::Type::Classic;

    string vertexSource = ReadSource(m_VertexFilepath);
    string fragmentSource = ReadSource(m_FragmentFilepath);

    auto vertexSource_ = static_cast <const GLchar*>(vertexSource.c_str());
    auto fragmentSource_ = static_cast <const GLchar*>(fragmentSource.c_str());

    GLuint vertexShaderId = Compile(vertexSource_, ShaderStage::Vertex, vertexFilepath);
    GLuint fragmentShaderId = Compile(fragmentSource_, ShaderStage::Fragment, fragmentFilepath);

    m_ShadersMap[ShaderStage::Vertex] = vertexShaderId;
    m_ShadersMap[ShaderStage::Fragment] = fragmentShaderId;

    glcheck( glCreateProgramPipelines(1, &m_PipelineId) );
    glcheck( glUseProgramStages(m_PipelineId, GL_VERTEX_SHADER_BIT, vertexShaderId) );
    glcheck( glUseProgramStages(m_PipelineId, GL_FRAGMENT_SHADER_BIT, fragmentShaderId) );

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

}