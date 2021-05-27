#pragma once
#include "../Core/Alias.h"

namespace FQW {

class ScriptableEntity;


class Script
{
public:
    void AttachEntity(Ref<ScriptableEntity> entity);

    virtual void FirstSetup() { }
    virtual void Start() { }
    virtual void Update(float deltaTime) { }

    static void Link(Ref<ScriptableEntity> entity, Ref<Script>);

protected:
    Ref<ScriptableEntity> m_Entity;
};



class ScriptableEntity
{
public:
    void AttachScript(Ref<Script> script);
    void DetachScript();

    void Start();
    void Update(float deltaTime);

    Ref<Script> m_BehaviourScript;
};

} // namespace FQW