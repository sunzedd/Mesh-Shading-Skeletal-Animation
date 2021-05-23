#pragma once
#include "../Graphics/IDrawable.h"
#include "../Core/Alias.h"
#include "BehaviourScript.h"

namespace FQW {

class Object : public IDrawable, public ScriptableEntity
{
public:
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;

public:
    Object();

    void Draw(Shader& shader, ICamera& camera);
    virtual void Update(float deltaTime) { }

    void PushDrawable(Ref<IDrawable> drawable);

    std::vector<Ref<IDrawable>> m_Drawables;
};

} // namespace FQW
