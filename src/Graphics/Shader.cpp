#include "Shader.h"


namespace FQW {

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    // Reading
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        FQW_ERROR("[Shader] Failed to read shader file");
        throw e;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compiling
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        FQW_ERROR("[Shader]: Failed to compile vertex shader {}", infoLog);
        throw std::runtime_error("Failed to compile vertex shader");
    };

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        FQW_ERROR("[Shader]: Failed to compile fragment shader {}", infoLog);
        throw std::runtime_error("Failed to compile fragment shader");
    };

    // Create ShaderProgram
    m_Id = glCreateProgram();
    glAttachShader(m_Id, vertex);
    glAttachShader(m_Id, fragment);
    glLinkProgram(m_Id);
    // print linking errors if any
    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
        FQW_ERROR("[Shader]: Failed to link shaders {}", infoLog);
        throw std::runtime_error("Failed to link shaders");
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    FQW_TRACE("Shader loaded succesfully. Vertex: {}, Fragment: {}", vertexPath, fragmentPath);
}


unsigned int Shader::GetUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_Id, name.c_str());
}


unsigned int Shader::GetAttributeLocation(const std::string& name) const
{
    return glGetAttribLocation(m_Id, name.c_str());
}


void Shader::Use()
{
    glUseProgram(m_Id);
}


void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
}


void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
}


void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
}

void Shader::SetMatrix4fv(const std::string& name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace FQW