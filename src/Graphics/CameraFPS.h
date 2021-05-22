#include "ICamera.h"


namespace FQW {

class CameraFPS : public ICamera
{
private:
    static constexpr float YAW = -90.0f;
    static constexpr float PITCH = 0.0f;
    static constexpr float SPEED = 2.5f;
    static constexpr float SENSITIVITY = 0.1f;
    static constexpr float ZOOM = 45.0f;

    enum MovementDirection {
        Forward,
        Backward,
        Left,
        Rigth
    };

public:
    CameraFPS(const glm::vec3& position = glm::vec3(0, 0, 0),
              const glm::vec3& up = glm::vec3(0, 1, 0),
              float yaw = YAW, float pitch = PITCH);

    glm::mat4 GetViewMatrix() const override;
    glm::mat4 GetProjectionMatrix() const override;

    // https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
    virtual void Update(float deltaTime) = 0;

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

    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;
};

} // namespace FQW