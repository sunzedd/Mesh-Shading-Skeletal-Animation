#include "CameraFPS.h"

namespace FQW {

CameraFPS::CameraFPS(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    :
    m_Position(position),
    m_Front(glm::vec3(0, 0, -1)),
    m_Up(up),
    m_WorldUp(glm::vec3(0, 1, 0)),
    m_Yaw(yaw),
    m_Pitch(pitch)
{
    UpdateVectors();
}


const glm::mat4& CameraFPS::GetViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}


const glm::mat4& CameraFPS::GetProjectionMatrix() const
{
    return glm::perspective(m_Fovy, m_AspectRatio, 0.001f, 1000.0f);
}


void CameraFPS::SetProjectionParameters(float aspectRatio, float fovy)
{
    m_AspectRatio = aspectRatio;
    m_Fovy = glm::radians(fovy);
}


void CameraFPS::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    UpdateVectors();
}


void CameraFPS::SetYaw(float yaw)
{
    m_Yaw = yaw;
    UpdateVectors();
}


void CameraFPS::SetPitch(float pitch)
{
    m_Pitch = pitch;
    UpdateVectors();
}


void CameraFPS::Move(CameraFPS::Direction direction, float velosity)
{
    switch (direction)
    {
        case Direction::Forward: m_Position += m_Front * velosity; break;
        case Direction::Backward: m_Position -= m_Front * velosity; break;
        case Direction::Rigth: m_Position += m_Right * velosity; break;
        case Direction::Left: m_Position -= m_Right * velosity; break;
        default: break;
    }
}


void CameraFPS::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp)); 
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

} // namespace FQW
