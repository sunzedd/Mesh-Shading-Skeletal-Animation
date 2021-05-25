#pragma once
#include "../../Animation/AnimatedModel.h"

namespace FQW {

class ModelScript : public Script
{
public:
    Ref<AnimatedModel> ThisEntity;

    void FirstSetup() override
    {
        ThisEntity = std::static_pointer_cast<AnimatedModel>(m_Entity);
    }

    void Start() override
    {
        ThisEntity->transform.rotation.z = 180.0f;
        ThisEntity->transform.rotation.y = 180.0f;
    }

    void Update(float deltaTime) override
    {
        ThisEntity->transform.rotation.y += deltaTime * 0.001;

        if (Input::IsKeyPressed(GLFW_KEY_P))
        {
            auto animator = ThisEntity->GetAnimator();
            if (!animator->IsPlaying())
            {
                animator->PlayAnimation();
                FQW_INFO("Playing animation");
            }
        }
    }
};

}