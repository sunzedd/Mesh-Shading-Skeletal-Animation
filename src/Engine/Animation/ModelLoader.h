#pragma once
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../EngineCore/Core/Alias.h"
#include "../EngineCore/Core/Input.h"
#include "../EngineCore/Graphics/Shader.h"
#include "../EngineCore/Graphics/IDrawable.h"

#include "../Animation/Mesh.h"

#include "Animation.h"
#include "Animator.h"
#include "Model.h"


namespace FQW {

/*
    Ключ = имя кости (Bone->name)
    Значение = пара (ID кости, Offset матрица кости)
*/
using BoneMap = std::unordered_map<std::string, std::pair<int, glm::mat4>>;


class ModelLoader 
{
public:
    static Ref<Model> LoadModel(std::string filepath);

private:
    static Ref<Model> ConstructModel(const aiScene* scene);


    static void RecursivelyLoadMeshAndBonesData(const aiNode* node,
                                                const aiScene* scene,
                                                std::vector<Ref<Mesh>>& outMeshes,
                                                BoneMap& outBoneMap);


    static Ref<Mesh> ProcessMeshAndUpdateBonemap(const aiMesh* assimpMesh, const aiScene* scene, BoneMap& outBoneMap);


    static void ExtractBonesWeights(std::vector<Vertex>& vertexBuffer,
                                    const aiMesh* assimpMesh,
                                    const aiScene* scene,
                                    BoneMap& outBoneMap);


    static bool ComposeSkeleton(const aiNode* node, BoneMap& boneMap, Bone& outBone);

    // Загрузка ключевых кадров анимации
    static void LoadAnimation(const aiScene* scene,
                              const aiAnimation* animation,
                              Animation& outAnimation);


    // Utils, checkers
    static void LoadMissingBones(const aiAnimation* assimpAnimation, BoneMap& outBoneMap);


    // Returns true if all vertices are affected
    static bool CheckIfAllVerticesSkinned(const Mesh& mesh);
};

} // namespace FQW 