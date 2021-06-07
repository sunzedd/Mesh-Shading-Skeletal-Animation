#pragma once
#include "ICamera.h"


namespace FQW {

class PerspectiveCamera : public ICamera
{
private:
    static constexpr float YAW = -90.0f;
    static constexpr float PITCH = 0.0f;

public:
    enum class Direction 
    {
        Forward,
        Backward,
        Left,
        Rigth
    };

public:
    PerspectiveCamera(const vec3& position = vec3(0, 0, 0), const vec3& up = vec3(0, 1, 0), 
                      float yaw = YAW, float pitch = PITCH);

    const mat4& GetViewMatrix() override;
    const mat4& GetProjectionMatrix() override;
    const mat4& GetViewProjectionMatrix() override;

    const vec3& GetPosition() const { return m_Position; }
    const vec3& GetFront() const { return m_Front; }
    const vec3& GetUp() const { return m_Up; }
    const vec3& GetRight() const { return m_Right; }

    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }

    void SetProjectionParameters(float aspectRatio, float fovy);
    void SetPosition(const glm::vec3& position);
    void SetYaw(float yaw);
    void SetPitch(float pitch);

    void Move(Direction direction, float velosity);

private:
    void UpdateVectors();

private:
    vec3 m_Position;
    vec3 m_Front;
    vec3 m_Up;
    vec3 m_Right;
    vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;

    float m_Fovy = 0.70f;
    float m_AspectRatio = 4.0f / 3.0f;

    mat4 m_ViewMatrix;
    mat4 m_ProjectionMatrix;
    mat4 m_ViewProjectionMatrix;
};

} // namespace FQW
