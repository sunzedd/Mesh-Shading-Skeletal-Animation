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

#include "Utility/AssimpGlmConverter.h"
#include "Core/Logger.h"


namespace FQW {

struct Vertex
{
    glm::vec3  position;
    glm::vec3  normal;
    glm::ivec4 boneIds = glm::ivec4(0);
    glm::vec4  boneWeights = glm::vec4(0.0f);
};


// —труктура, содержаща€ дерево костей (скелет)
struct Bone
{
    int id = 0;         // индекс позиции кости в финальном массиве, который будет отправлен в шейдер
    std::string name;
    glm::mat4 offset = glm::mat4(1.0f);
    std::vector<Bone> children = {};
};


// —труктура, представл€юща€ собой трек анимации
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
    std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
};



class AnimatedMesh
{
public:
    std::vector<Vertex> vertexBuffer;
    std::vector<uint32_t> indexBuffer;
    Bone skeleton;
    uint32_t boneCount = 0;

    AnimatedMesh(std::string filepath);

private:
    // «агрузка анимированной модели из asScene
    static void LoadMesh(const aiScene* scene, aiMesh* mesh,
                         std::vector<Vertex>& verticesOutput,
                         std::vector<uint32_t>& indicesOutput,
                         Bone& skeletonOutput,
                         uint32_t& nBoneCount);

    static bool ReadSkeleton(Bone& boneOutput,
                             aiNode* node,
                             std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfo);

    static void LoadAnimation(const aiScene* scene, Animation& animation);
};

} // namespace FQW