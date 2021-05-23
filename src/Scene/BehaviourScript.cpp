#pragma once
#include "BehaviourScript.h"

namespace FQW {

// ----------------------  BehaviourScript ----------------------
void BehaviourScript::AttachEntity(Ref<ScriptableEntity> entity)
{
    m_Entity = entity;
}

void BehaviourScript::Link(Ref<ScriptableEntity> entity, Ref<BehaviourScript> script)
{
    entity->AttachScript(script);
    script->AttachEntity(entity);
}


// ---------------------  ScriptableEntity ----------------------
void ScriptableEntity::AttachScript(Ref<BehaviourScript> script)
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
