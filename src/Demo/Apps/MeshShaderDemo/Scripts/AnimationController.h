#pragma once
#include <EngineCore/Core/Input.h>
#include <Scripts/ModelController.h>

namespace FQW::MeshShaderDemo {

class AnimationController : public ModelController
{
private:
    Ref<Animator> m_Animator;
    size_t m_AnimationCount = 0;
    int m_CurrentAnimationIndex = 0;

    float m_DeltaTime = 0;
    double m_LastAnimationSwitchTime = 0;
    
    bool m_RepeatAnimation = true;


public:
    void Start() override
    {
        m_Animator = This->GetAnimator();
        m_AnimationCount = This->GetAnimations().size();
    }

    void Update(float deltaTime) override
    {   
        m_DeltaTime = deltaTime;
        if (This->IsActive())
        {
            SwitchAnimation();
            PlayAnimation();
        }
    }

    void SwitchAnimation()
    {
        const vector<Ref<Animation>>& animations = This->GetAnimations();

        if (Input::IsKeyPressed(GLFW_KEY_LEFT)) 
        {
            const double time = Input::GetTime_ms();
            if (time - m_LastAnimationSwitchTime > 500.0f)
            {
                m_LastAnimationSwitchTime = Input::GetTime_ms();

                if (m_CurrentAnimationIndex == 0) {
                    m_CurrentAnimationIndex = m_AnimationCount - 1;
                }
                else {
                    m_CurrentAnimationIndex--;
                }
            }
        }
        else if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) 
        {
            const double time = Input::GetTime_ms();
            if (time - m_LastAnimationSwitchTime > 500.0f)
            {
                m_LastAnimationSwitchTime = Input::GetTime_ms();

                if (m_CurrentAnimationIndex == m_AnimationCount - 1) {
                    m_CurrentAnimationIndex = 0;
                }
                else {
                    m_CurrentAnimationIndex++;
                }
            }
        }
    }

    void PlayAnimation()
    {
        if (Input::IsKeyPressed(GLFW_KEY_P))
        {
            const vector<Ref<Animation>>& animations = This->GetAnimations();
            m_Animator->PlayAnimation(animations[m_CurrentAnimationIndex], m_RepeatAnimation);
        }
    }

    void OnDrawUI() override 
    {
        if (This->IsActive())
        {
            ImGui::Begin(u8"�������� ������");
            ImGui::Text(u8"�������� %d / %d", m_CurrentAnimationIndex + 1, m_AnimationCount);
            ImGui::Text(u8"����� %.3f / %.3f �", m_Animator->GetCurrentAnimationTime() / 1000.0f, m_Animator->GetEndOfAnimationsTime() / 1000.0f);
            ImGui::Checkbox("��������� ��������", &m_RepeatAnimation);
            ImGui::Separator();
            ImGui::Text(u8"<- / -> ����� ��������");
            ImGui::Text(u8"P - ��������� ��������");
            ImGui::End();
        }
    }
};

}
