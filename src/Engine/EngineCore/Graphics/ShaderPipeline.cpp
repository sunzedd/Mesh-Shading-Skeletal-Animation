#include "ShaderPipeline.h"

namespace FQW {

GLuint ShaderPipeline::GetUniformLocation(ShaderType shaderType, const string& name) const
{
    GLuint shaderId = m_ShadersMap.at(shaderType);
    OPENGL_CALL(unsigned int location = glGetUniformLocation(shaderId, name.c_str()));
    return location;
}


GLuint ShaderPipeline::GetAttributeLocation(ShaderType shaderType, const string& name) const
{
    GLuint shaderId = m_ShadersMap.at(shaderType);
    OPENGL_CALL(unsigned int location = glGetAttribLocation(shaderId, name.c_str()));
    return location;
}


void ShaderPipeline::Use()
{
    glUseProgram(0);
    OPENGL_CALL( glBindProgramPipeline(m_PipelineId) );
}


void ShaderPipeline::SetBool(ShaderType shaderType, const string& name, bool value) const
{
    uint32_t location = GetUniformLocation(shaderType, name);
    OPENGL_CALL( glProgramUniform1i(m_ShadersMap.at(shaderType), location, (int)value) );
    //OPENGL_CALL(glUniform1i(location, (int)value));
}


void ShaderPipeline::SetInt(ShaderType shaderType, const string& name, int value) const
{
    uint32_t location = GetUniformLocation(shaderType, name);
    OPENGL_CALL(glProgramUniform1i(m_ShadersMap.at(shaderType), location, (int)value));
    //OPENGL_CALL(glUniform1i(location, value));
}


void ShaderPipeline::SetFloat(ShaderType shaderType, const string& name, float value) const
{
    uint32_t location = GetUniformLocation(shaderType, name);
    OPENGL_CALL(glProgramUniform1f(m_ShadersMap.at(shaderType), location, value));
    //OPENGL_CALL(glUniform1f(location, value));
}

void ShaderPipeline::SetMatrix4fv(ShaderType shaderType, const string& name, const glm::mat4& value) const
{
    uint32_t location = GetUniformLocation(shaderType, name);
    OPENGL_CALL(glProgramUniformMatrix4fv(m_ShadersMap.at(shaderType), location, 1, GL_FALSE, glm::value_ptr(value)));
    //OPENGL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

void ShaderPipeline::SetMatrix4fvArray(ShaderType shaderType, const string& name,
                                       const std::vector<glm::mat4>& matrices) const
{
    uint32_t location = GetUniformLocation(shaderType, name);
    OPENGL_CALL(glProgramUniformMatrix4fv(
        m_ShadersMap.at(shaderType),
        location,
        matrices.size(),
        GL_FALSE,
        glm::value_ptr(matrices[0]))
    );

    //OPENGL_CALL(glUniformMatrix4fv(location, matrices.size(), GL_FALSE,
    //    glm::value_ptr(matrices[0])));
}


// protected section
string ShaderPipeline::ReadSource(string filepath)
{
    std::ifstream sourceFile;
    string source;

    sourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try 
    { 
        sourceFile.open(filepath); 
    }
    catch (std::ifstream::failure e)
    {
        FQW_ERROR("[Shader] Failed to read shader file {}", filepath);
        throw e;
    }

    std::stringstream sourceStringStream;
    sourceStringStream << sourceFile.rdbuf();

    sourceFile.close();

    source = sourceStringStream.str();
    return source;
}


GLuint ShaderPipeline::GetShaderIdByType(ShaderType type)
{
    return m_ShadersMap[type];
}


GLuint ShaderPipeline::Compile(const GLchar* source, ShaderType type)
{
    GLuint glType;

    switch (type)
    {
        case FQW::ShaderPipeline::Vertex:   glType = GL_VERTEX_SHADER; break;
        case FQW::ShaderPipeline::Fragment: glType = GL_FRAGMENT_SHADER; break;
        case FQW::ShaderPipeline::Mesh:     glType = GL_MESH_SHADER_NV; break;
        case FQW::ShaderPipeline::Task:     glType = GL_TASK_SHADER_NV; break;
        default:
            throw std::runtime_error("Invalid shader type passed");
            break;
    }

    OPENGL_CALL(GLuint shaderId = glCreateShaderProgramv(glType, 1, &source));

    GLint status;
    OPENGL_CALL(glGetProgramiv(shaderId, GL_LINK_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint logLen;
        glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &logLen);
        std::string log;
        if (logLen > 0)
        {
            log.resize(logLen, ' ');
            GLsizei written;
            glGetProgramInfoLog(shaderId, logLen, &written, &log[0]);
        }

        FQW_CRITICAL("Failed to link shader {}", log);
        throw std::runtime_error("Failed to link shader: " + log);
    }

    return shaderId;
}

}