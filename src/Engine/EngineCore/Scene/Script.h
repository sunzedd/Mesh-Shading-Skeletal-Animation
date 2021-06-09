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
    virtual void OnDrawUI() { }


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
    void OnDrawUI();

    bool HasScript() { return (bool)m_BehaviourScript; }
    bool IsActive() { return m_IsActive; }
    
    void Activate();
    void Deactivate();

protected:
    Ref<Script> m_BehaviourScript;
    bool m_IsActive = true;
};

} // namespace FQW