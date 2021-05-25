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

#include "../Core/Logger.h"
#include "../Graphics/OpenGL_Utils.h"
#include "../Utility/AssimpGlmConverter.h"


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
    // Mesh data
    std::vector<Vertex> m_VertexBuffer;
    std::vector<uint32_t> m_IndexBuffer;

    // Animation data
    Bone m_Skeleton;
    uint32_t m_BoneCount = 0;
    Animation m_Animation;
    std::vector<glm::mat4> m_Pose;
    glm::mat4 m_GlobalInverseTransform;

    AnimatedMesh(std::string filepath);

    uint32_t GetVertexArrayObject() const { return m_VertexArrayObject; }
    std::vector<glm::mat4>& GetCurrentPose(float time);

private:
    uint32_t m_VertexBufferObject = 0;
    uint32_t m_ElementBufferObject = 0;
    uint32_t m_VertexArrayObject = 0;

    // «агрузка анимированной модели из aiScene
    static void LoadMesh(const aiScene* scene, aiMesh* mesh,
                         std::vector<Vertex>& verticesOutput,
                         std::vector<uint32_t>& indicesOutput,
                         Bone& skeletonOutput,
                         uint32_t& nBoneCount);

    static bool ReadSkeleton(Bone& boneOutput,
                             aiNode* node,
                             std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfo);

    void LoadAnimation(const aiScene* scene, Animation& animation);

    void CreateVertexArrayObject();

    std::pair<uint32_t, float> GetTimeFraction(std::vector<float>& times,
                                               float currentTime);

    void CalculatePose(Animation& animation, Bone& skeletion,
                       float time, std::vector<glm::mat4>& output,
                       glm::mat4& parentTransform,
                       glm::mat4& globalInverseTransform);
};

} // namespace FQW