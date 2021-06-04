#pragma once
#include "../Core/IUpdatable.h"
#include "../Core/Alias.h"

namespace FQW {

class ScriptableEntity;


class Script : public IUpdatable
{
public:
    void AttachEntity(Ref<ScriptableEntity> entity);

    virtual void FirstSetup() { }
    virtual void Start() { }
    virtual void Update(float deltaTime) override { }

    static void Connect(Ref<ScriptableEntity> entity, Ref<Script> script);

protected:
    Ref<ScriptableEntity> m_Entity;
};



class ScriptableEntity : public IUpdatable
{
public:
    void AttachScript(Ref<Script> script);
    void DetachScript();

    void Start();
    void Update(float deltaTime) override;

    bool HasScript() { return (bool)m_BehaviourScript; }

    Ref<Script> m_BehaviourScript;
};

} // namespace FQW