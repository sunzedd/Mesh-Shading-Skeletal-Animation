#pragma once
#include "Animator.h"


namespace FQW {

class Model : public IDrawable, public ScriptableEntity
{
public:
    struct
    {
        vec3 position = vec3(0);
        vec3 rotation = vec3(0);
        vec3 scale = vec3(1, 1, 1); 
    } transform;


    Model(const vector<Ref<Mesh>>& meshes, vector<Ref<Animation>>& animations, Ref<Animator> animator);

    Ref<Animator> GetAnimator() const { return m_Animator; }
    const vector<Ref<Animation>>& GetAnimations() const { return m_Animations; }
    const vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    void Draw(ShaderPipeline& shader, ICamera& camera) override;


private:
    void BindShaderUniforms(ShaderPipeline& shader, ICamera& camera);

private:
    std::vector<Ref<Mesh>> m_Meshes;
    std::vector<Ref<Animation>> m_Animations;
    Ref<Animator> m_Animator;
};

} // namespace FQW