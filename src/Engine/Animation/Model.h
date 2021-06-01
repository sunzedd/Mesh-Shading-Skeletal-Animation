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

    void Draw(Shader& shader, ICamera& camera);
    void DrawWithMeshShader(Shader& shader, ICamera& camera);

    Ref<Animator> GetAnimator() const { return animator; }
    const std::vector<Ref<Animation>>& GetAnimations() { return animations; }
    const std::vector<Ref<Mesh>>& GetMeshes() { return meshes; }

private:
    void BindShaderUniforms(Shader& shader, ICamera& camera);

private:
    std::vector<Ref<Mesh>> meshes;
    std::vector<Ref<Animation>> animations;
    Ref<Animator> animator;

};

} // namespace FQW