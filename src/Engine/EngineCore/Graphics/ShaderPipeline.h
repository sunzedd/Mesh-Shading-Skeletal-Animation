#pragma once
#include <fstream>
#include <map>

#include "../Core/Alias.h"

#include "OpenGL_Utils.h"


namespace FQW {

class ShaderPipeline 
{
public:
    enum class ShaderStage
    {
        Vertex, Fragment,
        Mesh, Task
    };

    enum class Type
    {
        Classic,
        TuringRTX
    };


    void Use();

    ShaderPipeline::Type GetType() const { return m_Type; }
    bool TypeOf(ShaderPipeline::Type type) { return m_Type == type; }

    GLuint GetPipelineId() const { return m_PipelineId; }

    GLuint GetUniformLocation(ShaderStage stage, const string& name) const;
    GLuint GetAttributeLocation(ShaderStage stage, const string& name) const;

    void SetBool(ShaderStage stage, const string& name, bool value) const;
    void SetInt(ShaderStage stage, const string& name, int value) const;
    void SetFloat(ShaderStage stage, const string& name, float value) const;
    void SetVec3(ShaderStage stage, const string& name, const vec3& value) const;
    void SetMatrix4fv(ShaderStage stage, const string& name, const mat4& value) const;
    void SetMatrix4fvArray(ShaderStage stage, const string& name, const vector<mat4>& matrices) const;

protected:
    ShaderPipeline() {};

    static string ReadSource(string filepath);
    static GLuint Compile(const GLchar* source, ShaderStage stage, const string& filepath);

protected:
    Type m_Type;
    GLuint m_PipelineId;
    std::map<ShaderStage, GLuint> m_ShadersMap;
};

}