#pragma once
#include "AnimatedModel.h"


namespace FQW {

class Loader
{
public:
    static Ref<AnimatedModel> LoadAnimatedModel(std::string filepath);

    // ������ ������ ������������� ������ �� Assimp �����
    static Ref<AnimatedModel> ConstructAnimatedModel(const aiScene* scene);


private:
    // �������� ������������� ������ �� aiScene
    static void LoadMesh(
        const aiScene* scene,
        const aiMesh* mesh,
        std::vector<Vertex>& outVertexBuffer,
        std::vector<uint32_t>& outIndexBuffer,
        Bone& outSkeleton,
        uint32_t& outBoneCount);


    // ������ �������� ������
    static bool ReadSkeleton(
        Bone& boneOutput,
        const aiNode* node,
        std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfo);


    // �������� �������� ������ ��������
    static void LoadAnimation(
        const aiScene* scene,
        const aiAnimation* animation,
        Animation& outAnimation);

};

}