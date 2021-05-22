#pragma once
#include "../../Scene/BehaviourScript.h"
#include "../../Core/Input.h"
#include "../../Scene/CameraFPS.h"
#include "../../Scene/Object.h"

namespace FQW {

class ObjectScript : public BehaviourScript
{
private:
    Ref<Object> ThisObject;

public:
    void Start() { ThisObject = std::static_pointer_cast<Object>(m_Entity); }
    void Update(float deltaTime) { ThisObject->Rotation.y += deltaTime; }
};



class CameraScript : public BehaviourScript
{
private:
    Ref<CameraFPS> ThisCamera;
    float m_MouseSensitivity = 0.1f;
    float m_Speed = 2.5f;
    glm::vec2 m_LastMousePosition;
    bool m_IsMouseButtonPressed = false;

public:
    void Start() override
    {
        ThisCamera = std::static_pointer_cast<CameraFPS>(m_Entity);
        ThisCamera->SetPosition(glm::vec3(0, 0, 10));
    }

    void Update(float deltaTime) override
    {
        ProcessKeyboard(deltaTime);
        ProcessMouse();
    }

private:
    void ProcessKeyboard(float deltaTime)
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

    void ProcessMouse()
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
};

} // namespace FQW