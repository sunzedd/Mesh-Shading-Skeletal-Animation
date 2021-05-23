#pragma once
#include "../../Scene/BehaviourScript.h"
#include "../../Core/Input.h"
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

} // namespace FQW