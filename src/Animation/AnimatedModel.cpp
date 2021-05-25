#include "AnimatedModel.h"

namespace FQW {


AnimatedModel::AnimatedModel(Ref<Mesh> mesh, Ref<Animator> animator)
    :
    m_Mesh(mesh),
    m_Animator(animator)
{ }


void AnimatedModel::Draw(Shader& shader, ICamera& camera)
{
    SetupShaderUniforms(shader, camera);

    OPENGL_CALL(glBindVertexArray(m_Mesh->GetVertexArrayObject()));

    OPENGL_CALL(glDrawElements(
        GL_TRIANGLES,
        m_Mesh->GetIndexBuffer().size(),
        GL_UNSIGNED_INT, (GLvoid*)0));

    OPENGL_CALL(glBindVertexArray(0));
}


void AnimatedModel::SetupShaderUniforms(Shader& shader, ICamera& camera)
{
    glm::mat4 S = glm::mat4(1.0f);      // Scale
    glm::mat4 Rx = glm::mat4(1.0f);     // Rotate X
    glm::mat4 Ry = glm::mat4(1.0f);     // Rotate Y
    glm::mat4 Rz = glm::mat4(1.0f);     // Rotate Z
    glm::mat4 R = glm::mat4(1.0f);
    glm::mat4 T = glm::mat4(1.0f);      // Translate

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
    shader.SetMatrix4fvArray("bone_transforms", m_Animator->GetCurrentPose());
}


} // namespace FQW