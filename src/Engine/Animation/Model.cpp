#include "Model.h"

namespace FQW {

Model::Model(const std::vector<Ref<Mesh>>& meshes, std::vector<Ref<Animation>>& animations, Ref<Animator> animator)
{
    this->meshes = meshes;
    this->animations = animations;
    this->animator = animator;
}


void Model::Draw(Shader& shader, ICamera& camera)
{
    BindShaderUniforms(shader, camera);

    for (auto& mesh : meshes)
    {
        mesh->Draw();
    }
}

void Model::DrawWithMeshShader(Shader& shader, ICamera& camera)
{
    BindShaderUniforms(shader, camera);

    for (auto& mesh : meshes)
    {
        mesh->DrawWithMeshShader();
    }
}

void Model::BindShaderUniforms(Shader& shader, ICamera& camera)
{
    glm::mat4 S = glm::mat4(1.0f);
    glm::mat4 Rx = glm::mat4(1.0f);
    glm::mat4 Ry = glm::mat4(1.0f);
    glm::mat4 Rz = glm::mat4(1.0f);
    glm::mat4 R = glm::mat4(1.0f);
    glm::mat4 T = glm::mat4(1.0f);

    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 V = glm::mat4(1.0f);
    glm::mat4 P = glm::mat4(1.0f);
    glm::mat4 VP = glm::mat4(1.0f);

    S = glm::scale(S, transform.scale);
    Rx = glm::rotate(Rx, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
    Ry = glm::rotate(Ry, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
    Rz = glm::rotate(Rz, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
    R = Rx * Ry * Rz;
    T = glm::translate(T, transform.position);

    M = T * R * S;
    V = camera.GetViewMatrix();
    P = camera.GetProjectionMatrix();
    VP = P * V;

    shader.Use();
    shader.SetMatrix4fv("model_matrix", M);
    shader.SetMatrix4fv("view_projection_matrix", VP);

    const std::vector<glm::mat4>& pose = animator->GetCurrentPose();
    if (pose.size() > 0)
    {
        shader.SetMatrix4fvArray("bone_transforms", animator->GetCurrentPose());
    }
}

}