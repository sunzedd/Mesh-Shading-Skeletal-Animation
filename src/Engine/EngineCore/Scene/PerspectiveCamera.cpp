#include "PerspectiveCamera.h"


namespace FQW {

PerspectiveCamera::PerspectiveCamera(const vec3& position, const vec3& up, float yaw, float pitch)
    :
    m_Position(position),
    m_Front(vec3(0, 0, -1)),
    m_Up(up),
    m_WorldUp(vec3(0, 1, 0)),
    m_Yaw(yaw),
    m_Pitch(pitch)
{
    UpdateVectors();
}


const mat4& PerspectiveCamera::GetViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}


const mat4& PerspectiveCamera::GetProjectionMatrix() const
{
    return glm::perspective(m_Fovy, m_AspectRatio, 0.001f, 1000.0f);
}


void PerspectiveCamera::SetProjectionParameters(float aspectRatio, float fovy)
{
    m_AspectRatio = aspectRatio;
    m_Fovy = glm::radians(fovy);
}


void PerspectiveCamera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    UpdateVectors();
}


void PerspectiveCamera::SetYaw(float yaw)
{
    m_Yaw = yaw;
    UpdateVectors();
}


void PerspectiveCamera::SetPitch(float pitch)
{
    m_Pitch = pitch;
    UpdateVectors();
}


void PerspectiveCamera::Move(PerspectiveCamera::Direction direction, float velocity)
{
    switch (direction)
    {
        case Direction::Forward: m_Position += m_Front * velocity; break;
        case Direction::Backward: m_Position -= m_Front * velocity; break;
        case Direction::Rigth: m_Position += m_Right * velocity; break;
        case Direction::Left: m_Position -= m_Right * velocity; break;
        default: break;
    }
}


void PerspectiveCamera::UpdateVectors()
{
    vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp)); 
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

} // namespace FQW
