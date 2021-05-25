#include "Loader.h"

namespace FQW {

Ref<AnimatedModel> Loader::LoadAnimatedModel(std::string filepath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals);

    if (!scene ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
        FQW_ERROR("[Assimp] Failed to load model from {}.\nError: {}",
            filepath, importer.GetErrorString());

        throw std::runtime_error("[Assimp] Failed to load model from " + filepath);
    }

    auto result = ConstructAnimatedModel(scene);

    return result;
}


Ref<AnimatedModel> Loader::ConstructAnimatedModel(const aiScene* scene)
{
    // Currently read only first mesh and first animation
    const aiMesh* assimpMesh = scene->mMeshes[0];
    const aiAnimation* assimpAnimation = scene->mAnimations[0];

    std::vector<Vertex> vertexBuffer;
    std::vector<uint32_t> indexBuffer;
    Bone skeleton;
    uint32_t boneCount;
    Animation animation;

    LoadMesh(scene, assimpMesh, vertexBuffer, indexBuffer, skeleton, boneCount);
    LoadAnimation(scene, assimpAnimation, animation);

    auto mesh = CreateRef<Mesh>(vertexBuffer, indexBuffer);
    auto animator = CreateRef<Animator>(skeleton, boneCount, animation);
    auto resultModel = CreateRef<AnimatedModel>(mesh, animator);

    return resultModel;
}


void Loader::LoadMesh(
    const aiScene* scene,
    const aiMesh* mesh,
    std::vector<Vertex>& outVertexBuffer,
    std::vector<uint32_t>& outIndexBuffer,
    Bone& outSkeleton,
    uint32_t& outBoneCount)
{
    outVertexBuffer = {};
    outIndexBuffer = {};

    // Чтение вершин
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;

        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        v.normal.x = mesh->mNormals[i].x;
        v.normal.y = mesh->mNormals[i].y;
        v.normal.z = mesh->mNormals[i].z;

        v.boneIds = glm::ivec4(0);
        v.boneWeights = glm::vec4(0.0f);

        outVertexBuffer.push_back(v);
    }

    // Чтение костей
    std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};
    std::vector<uint32_t> boneCountForVertex(outVertexBuffer.size(), 0);

    outBoneCount = mesh->mNumBones;

    for (int i = 0; i < outBoneCount; i++)
    {
        aiBone* bone = mesh->mBones[i];
        glm::mat4 offsetMatrix = AssimpGlmConverter::AssimpMatrixToGlm(bone->mOffsetMatrix);
        boneInfo[bone->mName.C_Str()] = { i, offsetMatrix };

        for (int j = 0; j < bone->mNumWeights; j++)
        {
            uint32_t affectedVertexId = bone->mWeights[j].mVertexId; // Вершина, на которую воздействует кость bone
            float weight = bone->mWeights[j].mWeight;
            boneCountForVertex[affectedVertexId]++;

            switch (boneCountForVertex[affectedVertexId])
            {
                case 1:
                    outVertexBuffer[affectedVertexId].boneIds.x = i;
                    outVertexBuffer[affectedVertexId].boneWeights.x = weight;
                    break;
                case 2:
                    outVertexBuffer[affectedVertexId].boneIds.y = i;
                    outVertexBuffer[affectedVertexId].boneWeights.y = weight;
                    break;
                case 3:
                    outVertexBuffer[affectedVertexId].boneIds.z = i;
                    outVertexBuffer[affectedVertexId].boneWeights.z = weight;
                    break;
                case 4:
                    outVertexBuffer[affectedVertexId].boneIds.w = i;
                    outVertexBuffer[affectedVertexId].boneWeights.w = weight;
                    break;
                default:
                    break;
            }
        }
    }


    // Нормализация весов
    for (int i = 0; i < outVertexBuffer.size(); i++)
    {
        glm::vec4& boneWeights = outVertexBuffer[i].boneWeights;

        float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;

        if (totalWeight > 1.0f)
        {
            boneWeights /= totalWeight;
        }
    }


    // Чтение индексов
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++)
        {
            uint32_t idx = face.mIndices[j];
            outIndexBuffer.push_back(idx);
        }
    }


    // Заполнение скелета костями
    ReadSkeleton(outSkeleton, scene->mRootNode, boneInfo);
}


bool Loader::ReadSkeleton(
    Bone& boneOutput,
    const aiNode* node,
    std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfo)
{
    if (boneInfo.find(node->mName.C_Str()) != boneInfo.end())
    {
        boneOutput.name = node->mName.C_Str();
        boneOutput.id = boneInfo[boneOutput.name].first;
        boneOutput.offset = boneInfo[boneOutput.name].second;

        for (int i = 0; i < node->mNumChildren; i++)
        {
            Bone child;
            ReadSkeleton(child, node->mChildren[i], boneInfo);
            boneOutput.children.push_back(child);
        }

        return true;
    }
    else
    {
        for (int i = 0; i < node->mNumChildren; i++)
        {
            if (ReadSkeleton(boneOutput, node->mChildren[i], boneInfo))
            {
                return true;
            }
        }
    }

    return false;
}


void Loader::LoadAnimation(
    const aiScene* scene,
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

    outAnimation.duration = animation->mDuration / animation->mTicksPerSecond * 1000;
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
    }

    glm::mat4 globalInverseTransform = AssimpGlmConverter::AssimpMatrixToGlm(scene->mRootNode->mTransformation);
    outAnimation.globalInverseTransform = glm::inverse(globalInverseTransform);
}


} // namespace FQW