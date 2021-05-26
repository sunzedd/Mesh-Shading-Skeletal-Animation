#pragma once
#include "../../Core/Input.h"
#include "../../Animation/Model.h"
#include "../../Animation/Animator.h"

namespace FQW {

class ModelScript : public Script
{
public:
    Ref<Model> ThisEntity;

    void FirstSetup() override
    {
        ThisEntity = std::static_pointer_cast<Model>(m_Entity);
    }

    void Start() override
    {
        ThisEntity->transform.rotation.z = 180.0f;
        ThisEntity->transform.rotation.y = 180.0f;
    }

    void Update(float deltaTime) override
    {
        ThisEntity->transform.rotation.y += deltaTime * 0.001;
        auto animator = ThisEntity->GetAnimator();

        if (!animator->IsPlaying())
        {
            if (Input::IsKeyPressed(GLFW_KEY_1)) {
                animator->PlayAnimation(ThisEntity->GetAnimations()[0]);  FQW_INFO("Playing animation 0");
            }
            
            else if (Input::IsKeyPressed(GLFW_KEY_2)) {
                animator->PlayAnimation(ThisEntity->GetAnimations()[1]);  FQW_INFO("Playing animation 1");
            }
            else if (Input::IsKeyPressed(GLFW_KEY_3)) {
                animator->PlayAnimation(ThisEntity->GetAnimations()[2]);  FQW_INFO("Playing animation 2");
            }
        }
    }
};


}