#include "ClassicShaderPipeline.h"

namespace FQW {

ClassicShaderPipeline::ClassicShaderPipeline(string vertexFilepath, string fragmentFilepath)
    :
    m_VertexSourceFilepath(vertexFilepath),
    m_FragmentSourceFilepath(fragmentFilepath)
{
    string vertexSource = ReadSource(vertexFilepath);
    string fragmentSource = ReadSource(fragmentFilepath);

    auto pVertexSource = static_cast <const GLchar*>(vertexSource.c_str());
    auto pFragmentSource = static_cast <const GLchar*>(fragmentSource.c_str());

    GLuint vertexShaderId = Compile(pVertexSource, ShaderType::Vertex);
    GLuint fragmentShaderId = Compile(pFragmentSource, ShaderType::Fragment);

    m_ShadersMap[ShaderType::Vertex] = vertexShaderId;
    m_ShadersMap[ShaderType::Fragment] = fragmentShaderId;

    //glGenProgramPipelines(1, &m_PipelineId);
    OPENGL_CALL( glCreateProgramPipelines(1, &m_PipelineId) );
    OPENGL_CALL( glUseProgramStages(m_PipelineId, GL_VERTEX_SHADER_BIT, vertexShaderId) );
    OPENGL_CALL( glUseProgramStages(m_PipelineId, GL_FRAGMENT_SHADER_BIT, fragmentShaderId) );
}

}