#pragma once
#include "../EngineCore/Graphics/ShaderPipeline.h"


namespace FQW {


class TaskMeshShaderPipeline : public ShaderPipeline
{
public:
    TaskMeshShaderPipeline(const string& taskFilepath, const string& meshFilepath, const string& fragmentFilepath)
        :
        m_TaskFilepath(taskFilepath),
        m_MeshFilepath(meshFilepath),
        m_FragmentFilepath(fragmentFilepath)
    {
        m_Type = ShaderPipeline::Type::TuringRTX_TaskMesh;

        string taskSource = ReadSource(m_TaskFilepath);
        string meshSource = ReadSource(m_MeshFilepath);
        string fragmentSource = ReadSource(m_FragmentFilepath);

        auto taskSource_ = static_cast <const GLchar*>(taskSource.c_str());
        auto meshSource_ = static_cast <const GLchar*>(meshSource.c_str());
        auto fragmentSource_ = static_cast <const GLchar*>(fragmentSource.c_str());

        GLuint taskShaderId = Compile(taskSource_, ShaderStage::Task, m_TaskFilepath);
        GLuint meshShaderId = Compile(meshSource_, ShaderStage::Mesh, m_MeshFilepath);
        GLuint fragmentShaderId = Compile(fragmentSource_, ShaderStage::Fragment, m_FragmentFilepath);
        
        m_ShadersMap[ShaderStage::Task] = taskShaderId;
        m_ShadersMap[ShaderStage::Mesh] = meshShaderId;
        m_ShadersMap[ShaderStage::Fragment] = fragmentShaderId;

        glcheck(glCreateProgramPipelines(1, &m_PipelineId));
        glcheck(glUseProgramStages(m_PipelineId, GL_TASK_SHADER_BIT_NV, taskShaderId));
        glcheck(glUseProgramStages(m_PipelineId, GL_MESH_SHADER_BIT_NV, meshShaderId));
        glcheck(glUseProgramStages(m_PipelineId, GL_FRAGMENT_SHADER_BIT, fragmentShaderId));

        glDeleteShader(taskShaderId);
        glDeleteShader(meshShaderId);
        glDeleteShader(fragmentShaderId);
    }

private:
    string m_TaskFilepath;
    string m_MeshFilepath;
    string m_FragmentFilepath;
};


} // namespace FQW