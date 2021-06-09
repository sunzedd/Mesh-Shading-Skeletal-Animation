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
    float m_AnimationSpeedFactor = 1.0f;


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
            SwitchAnimationIfRequested();
            PlayAnimationIfRequested();
            m_Animator->SetAnimationSpeedFactor(m_AnimationSpeedFactor);
            m_Animator->Update(m_DeltaTime);
        }
    }

    void SwitchAnimationIfRequested()
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

    void PlayAnimationIfRequested()
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
            ImGui::Begin(u8"Анимация модели");
            ImGui::Text(u8"Анимация %d / %d", m_CurrentAnimationIndex + 1, m_AnimationCount);
            ImGui::Text(u8"Время %.3f / %.3f с", m_Animator->GetCurrentAnimationTime() / 1000.0f, m_Animator->GetEndOfAnimationsTime() / 1000.0f);
            ImGui::Checkbox(u8"Зациклить анимацию", &m_RepeatAnimation);
            ImGui::SliderFloat(u8"Скорость анимации", &m_AnimationSpeedFactor, 0.01f, 3.0f);
            ImGui::Separator();
            ImGui::Text(u8"<- / -> смена анимации");
            ImGui::Text(u8"P - проиграть анимацию");
            ImGui::End();
        }
    }
};

}
