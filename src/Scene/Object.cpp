#include "Object.h"

namespace FQW {

Object::Object()
    :
    Position(glm::vec3(0, 0, 0)),
    Rotation(glm::vec3(0, 0, 0)),
    Scale(glm::vec3(1, 1, 1))
{ }


void Object::Draw(Shader& shader, ICamera& camera)
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
    glm::mat4 MVP = glm::mat4(1.0f);

    S = glm::scale(S, Scale);
    Rx = glm::rotate(Rx, Rotation.x, glm::vec3(1, 0, 0));
    Ry = glm::rotate(Ry, Rotation.y, glm::vec3(0, 1, 0));
    Rz = glm::rotate(Rz, Rotation.z, glm::vec3(0, 0, 1));
    R = Rx * Ry * Rz;
    T = glm::translate(T, Position);

    M = T * R * S;
    V = camera.GetViewMatrix();
    P = camera.GetProjectionMatrix();
    MVP = P * V * M;

    shader.Use();
    shader.SetMatrix4fv("u_ModelMatrix", M);
    shader.SetMatrix4fv("u_MVP", MVP);

    for (Ref<IDrawable> drawable : m_Drawables)
    {
        drawable->Draw(shader, camera);
    }
}


void Object::PushDrawable(Ref<IDrawable> drawable)
{
    m_Drawables.push_back(drawable);
}


} // namespace FQW