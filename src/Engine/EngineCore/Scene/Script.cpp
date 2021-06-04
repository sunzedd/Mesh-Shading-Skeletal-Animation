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
    if (m_BehaviourScript)
    {
        m_BehaviourScript->Start();
    }
}


void ScriptableEntity::Update(float deltaTime)
{
    if (m_BehaviourScript)
    {
        m_BehaviourScript->Update(deltaTime);
    }
}

} // namespace FQW
