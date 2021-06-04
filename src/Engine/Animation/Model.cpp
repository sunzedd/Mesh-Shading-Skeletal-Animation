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
    mat4 S =  mat4(1.0f);
    mat4 Rx = mat4(1.0f);
    mat4 Ry = mat4(1.0f);
    mat4 Rz = mat4(1.0f);
    mat4 R =  mat4(1.0f);
    mat4 T =  mat4(1.0f);

    mat4 M =  mat4(1.0f);
    mat4 V =  mat4(1.0f);
    mat4 P =  mat4(1.0f);
    mat4 VP = mat4(1.0f);

    S = glm::scale(S, transform.scale);
    Rx = glm::rotate(Rx, glm::radians(transform.rotation.x), vec3(1, 0, 0));
    Ry = glm::rotate(Ry, glm::radians(transform.rotation.y), vec3(0, 1, 0));
    Rz = glm::rotate(Rz, glm::radians(transform.rotation.z), vec3(0, 0, 1));
    R = Rx * Ry * Rz;
    T = glm::translate(T, transform.position);

    M = T * R * S;
    V = camera.GetViewMatrix();
    P = camera.GetProjectionMatrix();
    VP = P * V;

    shaderPipleline.Use();
    shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderType::Vertex, "model_matrix", M);
    shaderPipleline.SetMatrix4fv(ShaderPipeline::ShaderType::Vertex, "view_projection_matrix", VP);

    const std::vector<mat4>& pose = m_Animator->GetCurrentPose();
    if (pose.size() > 0)
    {
        shaderPipleline.SetMatrix4fvArray(ShaderPipeline::ShaderType::Vertex, 
                                          "bone_transforms", 
                                          m_Animator->GetCurrentPose());
    }
}

}