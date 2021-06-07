#pragma once
#include <fstream>
#include <map>

#include "../Core/Alias.h"

#include "OpenGL_Utils.h"


namespace FQW {

class ShaderPipeline 
{
public:
    enum class ShaderType
    {
        Vertex, Fragment,
        Mesh, Task
    };


    void Use();

    GLuint GetPipelineId() const { return m_PipelineId; }

    GLuint GetUniformLocation(ShaderType shaderType, const string& name) const;
    GLuint GetAttributeLocation(ShaderType shaderType, const string& name) const;

    void SetBool(ShaderType shaderType, const string& name, bool value) const;
    void SetInt(ShaderType shaderType, const string& name, int value) const;
    void SetFloat(ShaderType shaderType, const string& name, float value) const;
    void SetVec3(ShaderType shaderType, const string& name, const vec3& value) const;
    void SetMatrix4fv(ShaderType shaderType, const string& name, const mat4& value) const;
    void SetMatrix4fvArray(ShaderType shaderType, const string& name, const vector<mat4>& matrices) const;

protected:
    ShaderPipeline() {};

    static string ReadSource(string filepath);
    static GLuint Compile(const GLchar* source, ShaderType type);

protected:
    GLuint m_PipelineId;
    std::map<ShaderType, GLuint> m_ShadersMap;
};

}