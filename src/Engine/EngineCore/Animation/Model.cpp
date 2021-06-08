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


void Model::BindShaderUniforms(ShaderPipeline& shaderPipleline, ICamera& camera)
{
    ShaderPipeline::ShaderStage stage = ShaderPipeline::ShaderStage::Vertex;
    switch (shaderPipleline.GetType())
    {
        case ShaderPipeline::Type::Classic:     stage = ShaderPipeline::ShaderStage::Vertex; break;
        case ShaderPipeline::Type::TuringRTX:   stage = ShaderPipeline::ShaderStage::Mesh; break;
        
        default: FQW_CRITICAL("Unknown shader pipeline type passed"); break;
    }
    
    const mat4& m = Transform.GetModelMatrix();
    const mat4& vp = camera.GetViewProjectionMatrix();
    mat4 mvp = vp * m;

    //shaderPipleline.Use();
    shaderPipleline.SetMatrix4fv(stage, "u_M_matrix", m);
    shaderPipleline.SetMatrix4fv(stage, "u_MVP_matrix", mvp);

    const vector<mat4>& pose = m_Animator->GetCurrentPose();
    if (pose.size() > 0) 
    {
        shaderPipleline.SetMatrix4fvArray(stage, "u_bone_transforms", m_Animator->GetCurrentPose());
    }
}

}