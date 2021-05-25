#pragma once
#include "Animation.h"
#include "../Graphics/Shader.h"
#include "../Graphics/IDrawable.h"
#include "../Core/Input.h"


namespace FQW {

class AnimatedModel : public IDrawable, public ScriptableEntity
{
public:
    Ref<AnimatedMesh> mesh;
    std::vector<glm::mat4> currentPose = {};

    struct {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 rotation = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
    } transform;

    AnimatedModel(Ref<AnimatedMesh> animatedMesh);

    void Draw(Shader& shader, ICamera& camera) override;
    void SetupShaderUniforms(Shader& shader, ICamera& camera);
};

} // namespace FQW