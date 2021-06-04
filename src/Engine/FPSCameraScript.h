#pragma once

#include "EngineCore/Scene/Script.h"
#include "EngineCore/Core/Input.h"
#include "EngineCore/Scene/PerspectiveCamera.h"

namespace FQW {

class FPSCameraScript : public Script
{
private:
    Ref<PerspectiveCamera> ThisCamera;
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