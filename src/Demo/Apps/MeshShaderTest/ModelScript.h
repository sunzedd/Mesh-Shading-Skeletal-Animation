#pragma once
#include <EngineCore/Core/Input.h>
#include <Animation/Model.h>
#include <Animation/Animator.h>

namespace FQW::MeshShaderTest {

class ModelScript : public Script
{
public:
    Ref<Model> ThisEntity;

    void FirstSetup() override;
    void Start() override;
    void Update(float deltaTime) override;
};

}