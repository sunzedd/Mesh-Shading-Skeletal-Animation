#pragma once
#include "Animator.h"

namespace FQW {

class Model : public IDrawable, public ScriptableEntity
{
public:
    struct {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 rotation = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
    } transform;


    Model(const std::vector<Ref<Mesh>>& meshes, std::vector<Ref<Animation>>& animations, Ref<Animator> animator);

    Ref<Animator> GetAnimator() const { return m_Animator; }
    const std::vector<Ref<Animation>>& GetAnimations() const { return m_Animations; }
    const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    void Draw(ShaderPipeline& shader, ICamera& camera) override;


private:
    void BindShaderUniforms(ShaderPipeline& shader, ICamera& camera);

private:
    std::vector<Ref<Mesh>> m_Meshes;
    std::vector<Ref<Animation>> m_Animations;
    Ref<Animator> m_Animator;
};

} // namespace FQW