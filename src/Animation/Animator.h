#pragma once
#include "Animation.h"
#include "Mesh.h"

namespace FQW {

class Animator
{
public:
    Animator(
        const Bone& skeleton,
        uint32_t boneCount,
        const Animation& animation);


    void PlayAnimation();


    bool IsPlaying() const { return m_IsPlaying; }
    

    void Update(float deltaTime);
    
    
    const std::vector<glm::mat4>& GetCurrentPose();


private:

    // Расчет текущей позы
    static void CalculatePose(
        const Animation& animation,
        const Bone& skeleton,
        float time,
        std::vector<glm::mat4>& outFinalTransforms,
        const glm::mat4& parentTransform,
        const glm::mat4& globalInverseTransform);


    // Вычисление временной дроби для интерполяции между кадрами
    static std::pair<uint32_t, float> GetTimeFraction(
        const std::vector<float>& times,
        float currentTime);


private:
    float m_CurrentAnimationTime = 0.0f;
    bool m_IsPlaying = false;

    std::vector<glm::mat4> m_CurrentPose;

    Bone m_Skeleton;
    Animation m_Animation;
    uint32_t m_BoneCount = 0;
};

}