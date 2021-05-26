#pragma once

#include "../Scene/Script.h"
#include "../Core/Input.h"
#include "../Scene/CameraFPS.h"

namespace FQW {

class CameraScript : public Script
{
private:
    Ref<CameraFPS> ThisCamera;
    float m_MouseSensitivity = 0.05f;
    float m_Speed = 0.01f;
    glm::vec2 m_LastMousePosition;
    bool m_IsMouseButtonPressed = false;

public:
    void FirstSetup() override;
    void Start() override;
    void Update(float deltaTime) override;

private:
    void ProcessKeyboard(float deltaTime);
    void ProcessMouse();
};

} // namespace FQW