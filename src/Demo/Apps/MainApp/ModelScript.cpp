#include "ModelScript.h"

namespace FQW::MainApp {

void ModelScript::FirstSetup()
{
    ThisEntity = std::static_pointer_cast<Model>(m_Entity);
}

void ModelScript::Start()
{
    ThisEntity->transform.position.y -= 4.0f;
    ThisEntity->transform.rotation.z = 180.0f;
    ThisEntity->transform.rotation.y = 180.0f;
}

void ModelScript::Update(float deltaTime)
{
    //ThisEntity->transform.rotation.y += deltaTime * 0.01;
    auto animator = ThisEntity->GetAnimator();


    if (!animator->IsPlaying())
    {
        static uint32_t animationsCount = ThisEntity->GetAnimations().size();
        static uint32_t currentAnimation = 0;

        if (Input::IsKeyPressed(GLFW_KEY_1)) 
        {
            FQW_INFO("Playing animation {}", currentAnimation);
            animator->PlayAnimation(ThisEntity->GetAnimations()[currentAnimation]);
            
            if (currentAnimation == animationsCount - 1)
            {
                currentAnimation = 0;
            }
            else
            {
                currentAnimation++;
            }
        }
    }
}

}