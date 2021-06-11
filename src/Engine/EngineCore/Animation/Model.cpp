#include "Model.h"


namespace FQW {

Model::Model(const vector<Ref<Mesh>>& meshes, vector<Ref<Animation>>& animations, Ref<Animator> animator)
    :
    m_Meshes(meshes),
    m_Animations(animations),
    m_Animator(animator)
{ }


void Model::Draw(ShaderPipeline& shaderPipeline, ICamera& camera)
{
    BindShaderUniforms(shaderPipeline, camera);
    for (auto& mesh : m_Meshes)
    {
        mesh->Draw(shaderPipeline, camera);
    }
}


// TODO:
// This is need to be rewritten.
// Probably this stuff must be done in kind of Renderer class
// That entity have as a component
void Model::BindShaderUniforms(ShaderPipeline& shaderPipleline, ICamera& camera)
{
    const mat4& m = Transform.GetModelMatrix();
    const mat4& vp = camera.GetViewProjectionMatrix();
    const mat4& v = camera.GetViewMatrix();
    mat4 mvp = vp * m;

    const vec3& viewDirection = camera.GetViewDirection();
    const vector<mat4>& pose = m_Animator->GetCurrentPose();

    switch (shaderPipleline.GetType())
    {
        case ShaderPipeline::Type::Classic:
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Vertex, "u_M_matrix", m);
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Vertex, "u_MVP_matrix", mvp);
            if (pose.size() > 0)
                shaderPipleline.SetMatrix4fvArray(ShaderPipeline::ShaderStage::Vertex, "u_bone_transforms", pose);
            break;

        case ShaderPipeline::Type::TuringRTX_Mesh:
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Mesh, "u_M_matrix", m);
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Mesh, "u_MVP_matrix", mvp);
            if (pose.size() > 0)
                shaderPipleline.SetMatrix4fvArray(ShaderPipeline::ShaderStage::Mesh, "u_bone_transforms", pose);
            break;

        case ShaderPipeline::Type::TuringRTX_TaskMesh:
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Task, "u_M_matrix", m);
            shaderPipleline.SetVec3(ShaderPipeline::ShaderStage::Task, "u_view_direction", viewDirection);
            shaderPipleline.SetVec3(ShaderPipeline::ShaderStage::Task, "u_camera_position", camera.GetPosition());
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Mesh, "u_MVP_matrix", mvp);
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Mesh, "u_V_matrix", v);
            shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderStage::Mesh, "u_M_matrix", m);
            if (pose.size() > 0)
                shaderPipleline.SetMatrix4fvArray(ShaderPipeline::ShaderStage::Mesh, "u_bone_transforms", pose);
            break;

        default:
            FQW_CRITICAL("Unknown shader pipeline type passed");
            break;
    }
}

}