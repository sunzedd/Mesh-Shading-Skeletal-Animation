#pragma once
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace FQW {

// Структура, содержащая дерево костей (скелет)
struct Bone
{
    int id = 0;         // индекс позиции кости в финальном массиве, который будет отправлен в шейдер
    std::string name;
    glm::mat4 offset = glm::mat4(1.0f);
    std::vector<Bone> children = {};
};


// Структура, представляющая собой трек анимации
struct BoneTransformTrack
{
    std::vector<float> positionTimestamps = {};
    std::vector<float> rotationTimestamps = {};
    std::vector<float> scaleTimestamps = {};

    std::vector<glm::vec3> positions = {};
    std::vector<glm::quat> rotations = {};
    std::vector<glm::vec3> scales = {};
};


struct Animation
{
    float duration = 0.0f;
    float ticksPerSecond = 1.0f;

    // Ключ - имя кости, значение - набор трансформаций кости в ключевых кадрах
    std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};

    glm::mat4 globalInverseTransform;
};


} // namespace FQW