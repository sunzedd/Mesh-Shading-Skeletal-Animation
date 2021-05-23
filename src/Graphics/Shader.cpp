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
    OPENGL_CALL( vertex = glCreateShader(GL_VERTEX_SHADER) );
    OPENGL_CALL( glShaderSource(vertex, 1, &vShaderCode, NULL) );
    OPENGL_CALL( glCompileShader(vertex) );
    OPENGL_CALL( glGetShaderiv(vertex, GL_COMPILE_STATUS, &success) );
    if (!success)
    {
        OPENGL_CALL(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
        FQW_ERROR("[Shader]: Failed to compile vertex shader {}", infoLog);
        throw std::runtime_error("Failed to compile vertex shader");
    } 
    else
    {
        FQW_TRACE("[Shader]: Compiled vertex shader successfully");
    }

    // Fragment Shader
    OPENGL_CALL( fragment = glCreateShader(GL_FRAGMENT_SHADER) );
    OPENGL_CALL( glShaderSource(fragment, 1, &fShaderCode, NULL) );
    OPENGL_CALL( glCompileShader(fragment) );
    OPENGL_CALL( glGetShaderiv(fragment, GL_COMPILE_STATUS, &success) );
    if (!success)
    {
        OPENGL_CALL( glGetShaderInfoLog(vertex, 512, NULL, infoLog) );
        FQW_ERROR("[Shader]: Failed to compile fragment shader {}", infoLog);
        throw std::runtime_error("Failed to compile fragment shader");
    }
    else
    {
        FQW_TRACE("[Shader]: Compiled fragment shader successfully");
    }

    // Create ShaderProgram
    OPENGL_CALL( m_Id = glCreateProgram() );
    OPENGL_CALL( glAttachShader(m_Id, vertex) );
    OPENGL_CALL( glAttachShader(m_Id, fragment) );
    OPENGL_CALL( glLinkProgram(m_Id) );

    // print linking errors if any
    OPENGL_CALL( glGetProgramiv(m_Id, GL_LINK_STATUS, &success) );
    if (!success)
    {
        OPENGL_CALL( glGetProgramInfoLog(m_Id, 512, NULL, infoLog) );
        FQW_ERROR("[Shader]: Failed to link shaders {}", infoLog);
        throw std::runtime_error("Failed to link shaders");
    }

    OPENGL_CALL( glDeleteShader(vertex) );
    OPENGL_CALL( glDeleteShader(fragment) );

    FQW_TRACE("Shader loaded succesfully. Vertex: {}, Fragment: {}", vertexPath, fragmentPath);
}


unsigned int Shader::GetUniformLocation(const std::string& name) const
{
     OPENGL_CALL( unsigned int location = glGetUniformLocation(m_Id, name.c_str()) );
     return location;
}


unsigned int Shader::GetAttributeLocation(const std::string& name) const
{
    OPENGL_CALL( unsigned int location = glGetAttribLocation(m_Id, name.c_str()) );
    return location;
}


void Shader::Use()
{
    OPENGL_CALL( glUseProgram(m_Id) );
}


void Shader::SetBool(const std::string& name, bool value) const
{
    unsigned int location = GetUniformLocation(name);
    OPENGL_CALL( glUniform1i(location, (int)value) );
}


void Shader::SetInt(const std::string& name, int value) const
{
    unsigned int location = GetUniformLocation(name);
    OPENGL_CALL( glUniform1i(location, value) );
}


void Shader::SetFloat(const std::string& name, float value) const
{
    unsigned int location = GetUniformLocation(name);
    OPENGL_CALL( glUniform1f(location, value) );
}

void Shader::SetMatrix4fv(const std::string& name, const glm::mat4 &value) const
{
    unsigned int location = GetUniformLocation(name);
    OPENGL_CALL( glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)) );
}

} // namespace FQW