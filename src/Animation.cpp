#include "Animation.h"

namespace FQW {

AnimatedMesh::AnimatedMesh(std::string filepath)
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

    aiMesh* mesh = scene->mMeshes[0];

    LoadMesh(scene, mesh, vertexBuffer, indexBuffer, skeleton, boneCount);
}


void AnimatedMesh::LoadMesh(const aiScene* scene, aiMesh* mesh,
                            std::vector<Vertex>& verticesOutput,
                            std::vector<uint32_t>& indicesOutput,
                            Bone& skeletonOutput,
                            uint32_t& nBoneCount)
{
    verticesOutput = {};
    indicesOutput = {};

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

        verticesOutput.push_back(v);
    }

    // Чтение костей
    std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};
    std::vector<uint32_t> boneCountForVertex(verticesOutput.size(), 0);

    nBoneCount = mesh->mNumBones;

    for (int i = 0; i < nBoneCount; i++)
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
                    verticesOutput[affectedVertexId].boneIds.x = i;
                    verticesOutput[affectedVertexId].boneWeights.x = weight;
                    break;
                case 2:
                    verticesOutput[affectedVertexId].boneIds.y = i;
                    verticesOutput[affectedVertexId].boneWeights.y = weight;
                    break;
                case 3:
                    verticesOutput[affectedVertexId].boneIds.z = i;
                    verticesOutput[affectedVertexId].boneWeights.z = weight;
                    break;
                case 4:
                    verticesOutput[affectedVertexId].boneIds.w = i;
                    verticesOutput[affectedVertexId].boneWeights.w = weight;
                    break;
                default:
                    break;
            }
        }
    }

    
    // Нормализация весов
    for (int i = 0; i < verticesOutput.size(); i++)
    {
        glm::vec4& boneWeights = verticesOutput[i].boneWeights;

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
            indicesOutput.push_back(idx);
        }
    }


    // Заполнение скелета костями
    ReadSkeleton(skeletonOutput, scene->mRootNode, boneInfo);
}


bool AnimatedMesh::ReadSkeleton(Bone& boneOutput,
                                aiNode* node,
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


void AnimatedMesh::LoadAnimation(const aiScene* scene, Animation& animation)
{
    
}

} // namespace FQW