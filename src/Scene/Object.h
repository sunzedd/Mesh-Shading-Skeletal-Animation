#pragma once
#include "../Graphics/IDrawable.h"

namespace FQW {

class Object : public IDrawable
{
public:
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;

public:
    Object();

    void Draw(Shader& shader, ICamera& camera);
    virtual void Update(float deltaTime) = 0;

    std::vector<IDrawable> m_Drawables;
};

} // namespace FQW
