#include "Model.h"


namespace FQW {

Model::Model(const vector<Ref<Mesh>>& meshes, vector<Ref<Animation>>& animations, Ref<Animator> animator)
    :
    m_Meshes(meshes),
    m_Animations(animations),
    m_Animator(animator)
{ }


void Model::Draw(ShaderPipeline& shader, ICamera& camera)
{
    BindShaderUniforms(shader, camera);
    for (auto& mesh : m_Meshes)
    {
        mesh->Draw();
    }
}


void Model::BindShaderUniforms(ShaderPipeline& shaderPipleline, ICamera& camera)
{
    const mat4& m = Transform.GetModelMatrix();
    const mat4& vp = camera.GetViewProjectionMatrix();
    mat4 mvp = vp * m;

    shaderPipleline.Use();
    shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderType::Vertex, "u_M_matrix", m);
    shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderType::Vertex, "u_VP_matrix", vp);
    shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderType::Vertex, "u_MVP_matrix", mvp);

    const vector<mat4>& pose = m_Animator->GetCurrentPose();
    if (pose.size() > 0)
    {
        shaderPipleline.SetMatrix4fvArray(
            ShaderPipeline::ShaderType::Vertex, 
            "u_bone_transforms", 
            m_Animator->GetCurrentPose());
    }
}

}