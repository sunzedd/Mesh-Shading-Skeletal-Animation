#pragma once
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "MeshOptimizer.h"

#include "../Core/Alias.h"
#include "../Core/Input.h"
#include "../Graphics/IDrawable.h"

#include "../Animation/Mesh.h"

#include "Animation.h"
#include "Animator.h"
#include "Model.h"

#include "../../MeshShading/NaiveMeshletBuilder.h"


namespace FQW {

/*
    Ключ = имя кости (Bone->name)
    Значение = пара (ID кости, Offset матрица кости)
*/
using BoneMap = std::unordered_map<string, std::pair<int, mat4>>;


class ModelLoader 
{
public:
    ModelLoader(MeshOptimizer& meshOptimizer, std::weak_ptr<IMeshletBuilder> meshletBuilder);
    Ref<Model> LoadModel(const string& filepath);

private:
    Ref<Model> ConstructModel(const aiScene* scene);


    void RecursivelyLoadMeshAndBonesData(const aiNode* node,
                                         const aiScene* scene,
                                         std::vector<Ref<Mesh>>& outMeshes,
                                         BoneMap& outBoneMap);


    Ref<Mesh> ProcessMeshAndUpdateBonemap(const aiMesh* assimpMesh, const aiScene* scene, BoneMap& outBoneMap);


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
    //static bool CheckIfAllVerticesSkinned(const Mesh& mesh);

private:
    MeshOptimizer& m_MeshOptimizer;
    std::weak_ptr<IMeshletBuilder> m_MeshletBuilder;
};

} // namespace FQW 