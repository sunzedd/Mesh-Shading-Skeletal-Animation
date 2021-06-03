#include "CameraScript.h"

namespace FQW {


void CameraScript::FirstSetup()
{
    ThisCamera = std::static_pointer_cast<CameraFPS>(m_Entity);
}

void CameraScript::Start()
{
    ThisCamera->SetPosition(glm::vec3(0, 0, 10));
}

void CameraScript::Update(float deltaTime)
{
    ProcessKeyboard(deltaTime);
    ProcessMouse();

    if (Input::IsKeyPressed(GLFW_KEY_EQUAL))
    {
        m_Speed += 0.005;
    }
    else if (Input::IsKeyPressed(GLFW_KEY_MINUS))
    {
        m_Speed -= 0.005;
    }
}

void CameraScript::ProcessKeyboard(float deltaTime)
{
    float velocity = m_Speed * deltaTime;
    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        ThisCamera->Move(CameraFPS::Direction::Forward, velocity);
    }
    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        ThisCamera->Move(CameraFPS::Direction::Backward, velocity);
    }
    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        ThisCamera->Move(CameraFPS::Direction::Left, velocity);
    }
    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        ThisCamera->Move(CameraFPS::Direction::Rigth, velocity);
    }
}

void CameraScript::ProcessMouse()
{
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && !m_IsMouseButtonPressed)
    {
        m_IsMouseButtonPressed = true;
        m_LastMousePosition = Input::GetCursorPosition();
    }

    if (!Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && m_IsMouseButtonPressed)
    {
        m_IsMouseButtonPressed = false;
    }

    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
    {
        glm::vec2 currentMousePosition = Input::GetCursorPosition();
        glm::vec2 offset = (currentMousePosition - m_LastMousePosition) * m_MouseSensitivity;

        float yaw = ThisCamera->GetYaw();
        float pitch = ThisCamera->GetPitch();

        yaw += offset.x;
        pitch -= offset.y;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f) { pitch = 89.0f; }
        if (pitch < -89.0f) { pitch = -89.0f; }

        ThisCamera->SetYaw(yaw);
        ThisCamera->SetPitch(pitch);

        m_LastMousePosition = currentMousePosition;
    }
}
}