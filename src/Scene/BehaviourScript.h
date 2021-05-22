#pragma once
#include "../Core/Alias.h"

namespace FQW {

class ScriptableEntity;


class BehaviourScript
{
public:
    void AttachEntity(Ref<ScriptableEntity> entity);

    virtual void Start() { }
    virtual void Update(float deltaTime) { }

    static void Link(Ref<ScriptableEntity> entity, Ref<BehaviourScript>);

protected:
    Ref<ScriptableEntity> m_Entity;
};



class ScriptableEntity
{
public:
    void AttachScript(Ref<BehaviourScript> script);
    void DetachScript();

    void Start();
    void Update(float deltaTime);

    Ref<BehaviourScript> m_BehaviourScript;
};

} // namespace FQW