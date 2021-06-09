#pragma once
#include "Script.h"

namespace FQW {

// ------------------------  Script------------------------------
void Script::AttachEntity(Ref<ScriptableEntity> entity)
{
    m_Entity = entity;
}

void Script::Connect(Ref<ScriptableEntity> entity, Ref<Script> script)
{
    entity->AttachScript(script);
    script->AttachEntity(entity);

    script->FirstSetup();
}


// ---------------------  ScriptableEntity ----------------------
void ScriptableEntity::AttachScript(Ref<Script> script)
{
    m_BehaviourScript = script;
}


void ScriptableEntity::DetachScript()
{
    m_BehaviourScript.reset();
}


void ScriptableEntity::Start()
{
    if (HasScript())
    {
        m_BehaviourScript->Start();
    }
}


void ScriptableEntity::Update(float deltaTime)
{
    if (HasScript())
    {
        m_BehaviourScript->Update(deltaTime);
    }
}


void ScriptableEntity::OnDrawUI()
{
    if (HasScript())
    {
        m_BehaviourScript->OnDrawUI();
    }
}


void ScriptableEntity::Activate()
{
    m_IsActive = true;
}


void ScriptableEntity::Deactivate()
{
    m_IsActive = false;
}

} // namespace FQW
