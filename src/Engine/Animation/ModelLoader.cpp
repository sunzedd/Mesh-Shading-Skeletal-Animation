#include "ModelLoader.h"

namespace FQW {

Ref<Model> ModelLoader::LoadModel(const string& filepath)
{
    FQW_INFO("\nLoading model from {}", filepath);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filepath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_OptimizeMeshes |
        aiProcess_GenSmoothNormals
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        FQW_ERROR("[Assimp] Failed to load model from {}.\nError: {}", filepath, importer.GetErrorString());
        throw std::runtime_error("[Assimp] Failed to load model from " + filepath);
    }

    auto result = ConstructModel(scene);

    return result;
}


Ref<Model> ModelLoader::ConstructModel(const aiScene* scene)
{
    std::vector<Ref<Mesh>> meshes;
    BoneMap boneMap;
    Bone skeletonRoot;

    const aiNode* rootNode = scene->mRootNode;

    // Mesh and bones hierarchy loading
    RecursivelyLoadMeshAndBonesData(rootNode, scene, meshes, boneMap);
    
    for (int i = 0; i < meshes.size(); i++)
    {
        if (!CheckIfAllVerticesSkinned(*meshes[i]))
        {
            FQW_WARN("Not all vertices skinned in mesh");
        }
    }


    // Animation loading
    std::vector<Ref<Animation>> animations;
    for (int i = 0; i < scene->mNumAnimations; i++)
    {
        const aiAnimation* assimpAnimation = scene->mAnimations[i];
        auto dstAnimation = CreateRef<Animation>();

        LoadAnimation(scene, assimpAnimation, *dstAnimation);
        animations.push_back(dstAnimation);
    }

    // Skeleton building
    ComposeSkeleton(rootNode, boneMap, skeletonRoot);

    // Model composing
    auto animator = CreateRef<Animator>(skeletonRoot, boneMap.size());
    auto result = CreateRef<Model>(meshes, animations, animator);

    FQW_INFO("Model loaded \n\t\tmeshes: {}\n\t\tanimations: {}", meshes.size(), animations.size());

    return result;
}


void ModelLoader::RecursivelyLoadMeshAndBonesData(const aiNode* node,
                                                  const aiScene* scene,
                                                  std::vector<Ref<Mesh>>& outMeshes,
                                                  BoneMap& outBoneMap)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        const aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[i]];
        Ref<Mesh> mesh = ProcessMeshAndUpdateBonemap(assimpMesh, scene, outBoneMap);
        outMeshes.push_back(mesh);
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        RecursivelyLoadMeshAndBonesData(node->mChildren[i], scene, outMeshes, outBoneMap);
    }
}


Ref<Mesh> ModelLoader::ProcessMeshAndUpdateBonemap(const aiMesh* assimpMesh,
                                                    const aiScene* scene,
                                                    BoneMap& outBoneMap)
{
    std::vector<Vertex> vertexBuffer;
    std::vector<uint32_t> indexBuffer;

    // Vertex data
    for (int i = 0; i < assimpMesh->mNumVertices; i++)
    {
        Vertex v;

        v.position = AssimpGlmConverter::AssimpVec3ToGlm(assimpMesh->mVertices[i]);
        v.normal = AssimpGlmConverter::AssimpVec3ToGlm(assimpMesh->mNormals[i]);

        vertexBuffer.push_back(v);
    }

    // Index data
    for (int i = 0; i < assimpMesh->mNumFaces; i++)
    {
        const aiFace& face = assimpMesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++)
        {
            uint32_t idx = face.mIndices[j];
            indexBuffer.push_back(idx);
        }
    }

    // Extract bones data
    ExtractBonesWeights(vertexBuffer, assimpMesh, scene, outBoneMap);


    auto resultMesh = CreateRef<Mesh>(vertexBuffer, indexBuffer);
    return resultMesh;
}


void ModelLoader::ExtractBonesWeights(std::vector<Vertex>& vertexBuffer,
                                       const aiMesh* assimpMesh,
                                       const aiScene* scene,
                                       BoneMap& outBoneMap)
{
    std::vector<uint32_t> boneCountForVertex(vertexBuffer.size(), 0);

    for (int i = 0; i < assimpMesh->mNumBones; i++)
    {
        const aiBone& assimpBone = *(assimpMesh->mBones[i]);
        FQW_TRACE("[Model loader] Bone {}", assimpBone.mName.C_Str());

        std::string name = assimpMesh->mBones[i]->mName.C_Str();
        if (name.empty()) {
            FQW_WARN("[ExtractBonesWeights] bone name is empty {}", name);
        }

        if (outBoneMap.find(name) == outBoneMap.end())
        {
            // Кость не найдена -> Добавляем ее в хеш-таблицу
            outBoneMap[name] = {
                outBoneMap.size(),
                AssimpGlmConverter::AssimpMatrixToGlm(assimpBone.mOffsetMatrix)
            };
        }

        
        for (int j = 0; j < assimpBone.mNumWeights; j++)
        {
            uint32_t affectedVertexId = assimpBone.mWeights[j].mVertexId;
            float weight = assimpBone.mWeights[j].mWeight;
            boneCountForVertex[affectedVertexId]++;

            switch (boneCountForVertex[affectedVertexId])
            {
                case 1:
                    vertexBuffer[affectedVertexId].boneIds.x = i;
                    vertexBuffer[affectedVertexId].boneWeights.x = weight;
                    break;
                case 2:
                    vertexBuffer[affectedVertexId].boneIds.y = i;
                    vertexBuffer[affectedVertexId].boneWeights.y = weight;
                    break;
                case 3:
                    vertexBuffer[affectedVertexId].boneIds.z = i;
                    vertexBuffer[affectedVertexId].boneWeights.z = weight;
                    break;
                case 4:
                    vertexBuffer[affectedVertexId].boneIds.w = i;
                    vertexBuffer[affectedVertexId].boneWeights.w = weight;
                    break;
                default:
                    break;
            }
        }
    }

#if 0
    for (uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
        uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
        float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
        Bones[VertexID].AddBoneData(BoneIndex, Weight);
    }
#endif

    // Нормализация весов
    for (int i = 0; i < vertexBuffer.size(); i++)
    {
        glm::vec4& weights = vertexBuffer[i].boneWeights;

        float totalWeight = weights.x + weights.y + weights.z + weights.w;
        if (totalWeight > 0.0f)
        {
            weights /= totalWeight;
        }
    }
}


bool ModelLoader::ComposeSkeleton(const aiNode* node, BoneMap& boneMap, Bone& outBone)
{
    if (boneMap.find(node->mName.C_Str()) != boneMap.end())     // если кость найдена
    {
        outBone.name = node->mName.C_Str();

        outBone.id = boneMap[outBone.name].first;
        outBone.offset = boneMap[outBone.name].second;

        for (int i = 0; i < node->mNumChildren; i++)
        {
            Bone child;
            ComposeSkeleton(node->mChildren[i], boneMap, child);
            outBone.children.push_back(child);
        }

        return true;
    }
    else
    {
        FQW_WARN("[ComposeSkeleton] Not found bone in BoneMap. Node name = {}", node->mName.C_Str());

        for (int i = 0; i < node->mNumChildren; i++)
        {
            if (ComposeSkeleton(node->mChildren[i], boneMap, outBone))
            {
                return true;
            }
        }
    }

    return false;
}


// Загрузка ключевых кадров анимации
void ModelLoader::LoadAnimation(const aiScene* scene,
                                const aiAnimation* animation,
                                Animation& outAnimation)
{
    if (animation->mTicksPerSecond != 0.0f)
    {
        outAnimation.ticksPerSecond = animation->mTicksPerSecond;
    }
    else
    {
        outAnimation.ticksPerSecond = 1;
    }

    outAnimation.duration = animation->mDuration;
    outAnimation.boneTransforms = {};

    // load positions rotations and scales for each bone
    // each channel represents each bone
    for (int i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim* channel = animation->mChannels[i];
        BoneTransformTrack track;

        for (int j = 0; j < channel->mNumPositionKeys; j++)
        {
            float positionTimestamp = channel->mPositionKeys[j].mTime;
            track.positionTimestamps.push_back(positionTimestamp);

            glm::vec3 position = AssimpGlmConverter::AssimpVec3ToGlm(channel->mPositionKeys[j].mValue);
            track.positions.push_back(position);
        }

        for (int j = 0; j < channel->mNumRotationKeys; j++)
        {
            track.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
            track.rotations.push_back(AssimpGlmConverter::AssimpQuaternionToGlm(channel->mRotationKeys[j].mValue));

        }

        for (int j = 0; j < channel->mNumScalingKeys; j++)
        {
            track.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
            track.scales.push_back(AssimpGlmConverter::AssimpVec3ToGlm(channel->mScalingKeys[j].mValue));
        }

        outAnimation.boneTransforms[channel->mNodeName.C_Str()] = track;

        if (std::string(channel->mNodeName.C_Str()).empty()) 
        {
            FQW_WARN("[LoadAnimation] channel (bone) name is empty");
        }
    }

    glm::mat4 globalInverseTransform = AssimpGlmConverter::AssimpMatrixToGlm(scene->mRootNode->mTransformation);
    outAnimation.globalInverseTransform = glm::inverse(globalInverseTransform);
}


void ModelLoader::LoadMissingBones(const aiAnimation* assimpAnimation, BoneMap& outBoneMap)
{
    for (int i = 0; i < assimpAnimation->mNumChannels; i++)
    {
        const aiNodeAnim* chanel = assimpAnimation->mChannels[i];
        std::string boneName = chanel->mNodeName.C_Str();

        if (outBoneMap.find(boneName) == outBoneMap.end())
        {
            outBoneMap[boneName] = { outBoneMap.size(), glm::mat4(1.0f) };
        }
    }
}


bool ModelLoader::CheckIfAllVerticesSkinned(const Mesh& mesh)
{
    const auto& vertices = mesh.GetVertexBuffer();
    bool allAffected = true;

    for (int i = 0; i < vertices.size(); i++)
    {
        const Vertex& v = vertices[i];

        if (v.boneIds.x == -1) { allAffected = false; break; }
        if (v.boneIds.y == -1) { allAffected = false; break; }
        if (v.boneIds.z == -1) { allAffected = false; break; }
        if (v.boneIds.w == -1) { allAffected = false; break; }
    }

    return allAffected;
}

}