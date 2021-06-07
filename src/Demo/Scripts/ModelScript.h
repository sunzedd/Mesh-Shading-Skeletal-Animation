#pragma once
#include <EngineCore/Core/Input.h>
#include <EngineCore/Animation/Model.h>
#include <EngineCore/Animation/Animator.h>

namespace FQW::MainApp {

class ModelScript : public Script
{
public:
    Ref<Model> ThisEntity;

    void FirstSetup() override;
    void Start() override;
    void Update(float deltaTime) override;
};

}