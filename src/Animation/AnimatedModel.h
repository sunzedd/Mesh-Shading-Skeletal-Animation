#pragma once
#include "Mesh.h"
#include "Animator.h"

#include "../Graphics/Shader.h"
#include "../Graphics/IDrawable.h"
#include "../Core/Input.h"


namespace FQW {


class AnimatedModel : public IDrawable, public ScriptableEntity
{
public:
    struct
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 rotation = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
    } transform;

    AnimatedModel(Ref<Mesh> mesh, Ref<Animator> animator);

    void Draw(Shader& shader, ICamera& camera) override;

    Ref<Animator>& GetAnimator() { return m_Animator; }

private:
    Ref<Mesh> m_Mesh;
    Ref<Animator> m_Animator;

    void SetupShaderUniforms(Shader& shader, ICamera& camera);
};


} // namespace FQW