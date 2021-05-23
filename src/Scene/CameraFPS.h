#include "ICamera.h"
#include "BehaviourScript.h"


namespace FQW {

class CameraFPS : public ICamera
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
    CameraFPS(const glm::vec3& position = glm::vec3(0, 0, 0),
              const glm::vec3& up = glm::vec3(0, 1, 0),
              float yaw = YAW, float pitch = PITCH);

    const glm::mat4& GetViewMatrix() const override;
    const glm::mat4& GetProjectionMatrix() const override;
    const glm::vec3& GetPosition() const { return m_Position; }
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
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;

    float m_Fovy = 0.70f;
    float m_AspectRatio = 4.0f / 3.0f;
};

} // namespace FQW
