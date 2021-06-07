#include "ShaderPipeline.h"

namespace FQW {

GLuint ShaderPipeline::GetUniformLocation(ShaderType shaderType, const string& name) const
{
    const GLchar* name_ = static_cast<const GLchar*>(name.c_str());
    GLuint shaderId = m_ShadersMap.at(shaderType);

    glcheck(GLuint location = glGetUniformLocation(shaderId, name_));
    if (location < 0 || location > GL_MAX_UNIFORM_LOCATIONS)
    {
        FQW_WARN("Uniform not found {} in shader {}", name, shaderType);
    }

    return location;
}


GLuint ShaderPipeline::GetAttributeLocation(ShaderType shaderType, const string& name) const
{
    GLuint shaderId = m_ShadersMap.at(shaderType);
    glcheck(GLuint location = glGetAttribLocation(shaderId, name.c_str()));
    return location;
}


void ShaderPipeline::Use()
{
    glUseProgram(0);
    glcheck( glBindProgramPipeline(m_PipelineId) );
}


void ShaderPipeline::SetBool(ShaderType shaderType, const string& name, bool value) const
{
    GLuint location = GetUniformLocation(shaderType, name);
    glcheck( glProgramUniform1i(m_ShadersMap.at(shaderType), location, (int)value) );
}


void ShaderPipeline::SetInt(ShaderType shaderType, const string& name, int value) const
{
    GLuint location = GetUniformLocation(shaderType, name);
    glcheck(glProgramUniform1i(m_ShadersMap.at(shaderType), location, (int)value));
}


void ShaderPipeline::SetFloat(ShaderType shaderType, const string& name, float value) const
{
    GLuint location = GetUniformLocation(shaderType, name);
    glcheck(glProgramUniform1f(m_ShadersMap.at(shaderType), location, value));
}

void ShaderPipeline::SetVec3(ShaderType shaderType, const string& name, const vec3& value) const
{
    GLuint location = GetUniformLocation(shaderType, name);
    glcheck(glProgramUniform3fv(m_ShadersMap.at(shaderType), location, 1, glm::value_ptr(value)));
}

void ShaderPipeline::SetMatrix4fv(ShaderType shaderType, const string& name, const mat4& value) const
{
    GLuint location = GetUniformLocation(shaderType, name);
    glcheck(glProgramUniformMatrix4fv(m_ShadersMap.at(shaderType), location, 1, GL_FALSE, glm::value_ptr(value)));
}


void ShaderPipeline::SetMatrix4fvArray(ShaderType shaderType, const string& name, const vector<mat4>& matrices) const
{
    GLuint location = GetUniformLocation(shaderType, name);
    glcheck(glProgramUniformMatrix4fv(m_ShadersMap.at(shaderType), location, matrices.size(), GL_FALSE,
                                      glm::value_ptr(matrices[0])) );
}


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



GLuint ShaderPipeline::Compile(const GLchar* source, ShaderType type, const string& filepath)
{
    GLuint glType;

    switch (type)
    {
        case FQW::ShaderPipeline::ShaderType::Vertex:   glType = GL_VERTEX_SHADER;   break;
        case FQW::ShaderPipeline::ShaderType::Fragment: glType = GL_FRAGMENT_SHADER; break;
        case FQW::ShaderPipeline::ShaderType::Mesh:     glType = GL_MESH_SHADER_NV;  break;
        case FQW::ShaderPipeline::ShaderType::Task:     glType = GL_TASK_SHADER_NV;  break;
        default:
            FQW_CRITICAL("Invalid shader type passed");
            throw std::runtime_error("Invalid shader type passed");
            break;
    }

    glcheck(GLuint shaderId = glCreateShaderProgramv(glType, 1, &source));

    GLint status;
    glcheck(glGetProgramiv(shaderId, GL_LINK_STATUS, &status));
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

        FQW_CRITICAL("Failed to link shader {}, Info: {}", filepath, log);
        throw std::runtime_error("Failed to link shader: " + log);
    }

    return shaderId;
}

}