#pragma once
#include "Animation.h"
#include "Mesh.h"

namespace FQW {


class Animator : public IUpdatable
{
public:
    Animator(const Bone& skeleton, uint32_t boneCount);

    void PlayAnimation(Ref<Animation> animation, bool repeat=false);
    void Update(float deltaTime);
    void CalculateCurrentPose();
    bool IsPlaying() const { return m_IsPlaying; }

    const vector<mat4>& GetCurrentPose() const { return m_CurrentPose; }
    const Bone& GetSkeleton() const { return m_Skeleton; }
    float GetCurrentAnimationTime() const { return m_CurrentAnimationTime; }
    float GetEndOfAnimationsTime()  const { return m_EndOfAnimationTime; }
    float GetAnimationSpeedFactor() const { return m_AnimationSpeedFactor; }

    void SetAnimationSpeedFactor(float value);

private:
    // Расчет текущей позы
    static void CalculatePose(
        const Animation& animation,
        const Bone& bone,
        float time,
        std::vector<glm::mat4>& outFinalTransforms,
        const glm::mat4& parentTransform,
        const glm::mat4& globalInverseTransform);


    // Вычисление временной дроби для интерполяции между кадрами
    static std::pair<uint32_t, float> GetTimeFraction(const std::vector<float>& times, float& dt);


private:
    float m_CurrentAnimationTime = 0.0f;
    float m_EndOfAnimationTime = 0.0f;
    float m_AnimationSpeedFactor = 1.0f;
    bool m_IsPlaying = false;
    bool m_Repeat = false;

    std::vector<glm::mat4> m_CurrentPose;

    Bone m_Skeleton;
    Ref<Animation> m_Animation;
    uint32_t m_BoneCount = 0;
};

} // namespace FQW