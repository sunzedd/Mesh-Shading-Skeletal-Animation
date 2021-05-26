#include "Animator.h"

namespace FQW {

Animator::Animator(const Bone& skeleton, uint32_t boneCount)
    :
    m_Skeleton(skeleton)
{
    m_CurrentPose.resize(boneCount, glm::mat4(1.0f));
}


void Animator::PlayAnimation(Ref<Animation> animation)
{
    m_Animation = animation;
    m_CurrentAnimationTime = 0.001f;
    m_IsPlaying = true;
}


void Animator::Update(float deltaTime)
{
    if (!m_Animation) return;

    if (m_CurrentAnimationTime >= m_Animation->duration)
    {
        m_IsPlaying = false;
    }

    if (m_IsPlaying)
    {
        m_CurrentAnimationTime += deltaTime;
    }
}


const std::vector<glm::mat4>& Animator::GetCurrentPose()
{
    if (m_IsPlaying)
    {
        CalculatePose(*m_Animation, m_Skeleton, m_CurrentAnimationTime,
                      m_CurrentPose, glm::mat4(1.0f), 
                      m_Animation->globalInverseTransform);
    }

    return m_CurrentPose;
}



// Расчет текущей позы
void Animator::CalculatePose(
    const Animation& animation,
    const Bone& bone,
    float time,
    std::vector<glm::mat4>& outFinalTransforms,
    const glm::mat4& parentTransform,
    const glm::mat4& globalInverseTransform)
{
    auto found = animation.boneTransforms.find(bone.name);
    if (found == animation.boneTransforms.end())
    {
        FQW_WARN("Failed to get bone transform for the name {}", bone.name);
        return;
    }
        
    const BoneTransformTrack& btt = found->second;

    glm::mat4 globalTransform;

    try {
        time = fmod(time, animation.duration);
        std::pair<uint32_t, float> fp;

        //calculate interpolated position
        glm::vec3 position1;

        fp = GetTimeFraction(btt.positionTimestamps, time);
        //if (fp.first == 0) { position1 = glm::vec3(1.0f); }
        //else { glm::vec3 position1 = btt.positions[fp.first - 1]; }
        position1 = btt.positions[fp.first - 1];
        glm::vec3 position2 = btt.positions[fp.first];
        glm::vec3 position = glm::mix(position1, position2, fp.second);

        //calculate interpolated rotation
        glm::quat rotation1;
        fp = GetTimeFraction(btt.rotationTimestamps, time);
        //if (fp.first == 0) { rotation1 = glm::quat(glm::vec3(0.0f)); }
        //else { rotation1 = btt.rotations[fp.first - 1]; }
        rotation1 = btt.rotations[fp.first - 1];
        glm::quat rotation2 = btt.rotations[fp.first];
        glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);

        //calculate interpolated scale
        glm::vec3 scale1;
        fp = GetTimeFraction(btt.scaleTimestamps, time);
        //if (fp.first == 0) { scale1 = glm::vec3(1.0f); }
        //else { scale1 = btt.scales[fp.first - 1]; }
        scale1 = btt.scales[fp.first - 1];
        glm::vec3 scale2 = btt.scales[fp.first];
        glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

        glm::mat4 positionMat = glm::mat4(1.0);
        glm::mat4 scaleMat = glm::mat4(1.0);

        // calculate localTransform
        positionMat = glm::translate(positionMat, position);
        glm::mat4 rotationMat = glm::toMat4(rotation);
        scaleMat = glm::scale(scaleMat, scale);

        glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
        globalTransform = parentTransform * localTransform;

        outFinalTransforms[bone.id] = globalInverseTransform * globalTransform * bone.offset;
    }
    catch (std::exception& e)
    {
        FQW_ERROR("Exception: {}", e.what());
        globalTransform = glm::mat4(1.0f);
    }

    //update values for children bones
    for (const Bone& child : bone.children)
    {
        CalculatePose(animation, child, time, outFinalTransforms, globalTransform, globalInverseTransform);
    }
}


// Вычисление временной дроби для интерполяции между кадрами
std::pair<uint32_t, float> Animator::GetTimeFraction(
    const std::vector<float>& times,
    float currentTime)

{
    if (currentTime > times[times.size() - 1]) __debugbreak();

    uint32_t segment = 0;
    while (currentTime > times[segment])
    {
        segment++;
    }

    float start;
    float end;
    float frac;

    if (times.size() == 1)
    {
        start = currentTime;
        end = times[0];
    }
    else
    {
        if (segment == 0)
        {
            start = times[segment];
            end = times[segment + 1];
        }
        else
        {
            start = times[segment - 1];
            end = times[segment];
        }
    }
    
    frac = (currentTime - start) / (end - start);
    //float start = times[segment - 1];
    //float end = times[segment];
    //float frac = (currentTime - start) / (end - start);

    return { segment, frac };
}


} // namespace FQW