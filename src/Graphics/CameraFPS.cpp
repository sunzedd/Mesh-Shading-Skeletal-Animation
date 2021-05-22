#include "CameraFPS.h"

namespace FQW {

CameraFPS::CameraFPS(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    :
    m_Position(position),
    m_Front(glm::vec3(0, 0, -1)),
    m_Up(up),
    m_Yaw(yaw),
    m_Pitch(pitch),
    m_MovementSpeed(SPEED),
    m_MouseSensitivity(SENSITIVITY),
    m_Zoom(ZOOM)
{
    UpdateVectors();
}


glm::mat4 CameraFPS::GetViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}


glm::mat4 CameraFPS::GetProjectionMatrix() const
{
    return glm::perspective(0.60f, 16.0f / 10.0f, 0.001f, 1000.0f);
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

}