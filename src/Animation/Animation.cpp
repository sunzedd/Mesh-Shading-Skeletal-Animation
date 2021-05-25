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

    LoadMesh(scene, mesh, m_VertexBuffer, m_IndexBuffer, m_Skeleton, m_BoneCount);
    LoadAnimation(scene, m_Animation);
    CreateVertexArrayObject();

    FQW_TRACE("[Assimp] Loaded Mesh");
    FQW_TRACE("[Assimp] Loaded Animation");

    m_GlobalInverseTransform = AssimpGlmConverter::AssimpMatrixToGlm(scene->mRootNode->mTransformation);
    m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
}


std::vector<glm::mat4>& AnimatedMesh::GetCurrentPose(float time)
{
    CalculatePose(m_Animation, m_Skeleton, time, m_Pose, glm::mat4(1.0f), m_GlobalInverseTransform);
    return m_Pose;
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
    aiAnimation* anim = scene->mAnimations[0];

    if (anim->mTicksPerSecond != 0.0f)
    {
        animation.ticksPerSecond = anim->mTicksPerSecond;
    }
    else
    {
        animation.ticksPerSecond = 1;
    }

    animation.duration = anim->mDuration * anim->mTicksPerSecond;
    animation.boneTransforms = {};

    //load positions rotations and scales for each bone
    // each channel represents each bone
    for (int i = 0; i < anim->mNumChannels; i++) 
    {
        aiNodeAnim* channel = anim->mChannels[i];
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
        
        animation.boneTransforms[channel->mNodeName.C_Str()] = track;
    }

    m_Pose.resize(m_BoneCount, glm::mat4(1.0f));
}


void AnimatedMesh::CreateVertexArrayObject()
{
    // Buffers
    OPENGL_CALL(glGenVertexArrays(1, &m_VertexArrayObject));
    OPENGL_CALL(glGenBuffers(1, &m_VertexBufferObject));
    OPENGL_CALL(glGenBuffers(1, &m_ElementBufferObject));

    OPENGL_CALL(glBindVertexArray(m_VertexArrayObject));

    OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject));
    OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER,
                             m_VertexBuffer.size() * sizeof(Vertex),
                             &(m_VertexBuffer[0]), GL_STATIC_DRAW));

    OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObject));
    OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             m_IndexBuffer.size() * sizeof(uint32_t),
                             &(m_IndexBuffer[0]), GL_STATIC_DRAW));

    // Vertex attribute layout
    OPENGL_CALL(glEnableVertexAttribArray(0));
    OPENGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));

    OPENGL_CALL(glEnableVertexAttribArray(1));
    OPENGL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (GLvoid*)offsetof(Vertex, normal)));

    OPENGL_CALL(glEnableVertexAttribArray(2));
    OPENGL_CALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (GLvoid*)offsetof(Vertex, boneIds)));

    OPENGL_CALL(glEnableVertexAttribArray(3));
    OPENGL_CALL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (GLvoid*)offsetof(Vertex, boneWeights)));

    OPENGL_CALL(glBindVertexArray(0));
}


std::pair<uint32_t, float> AnimatedMesh::GetTimeFraction(std::vector<float>& times,
                                                         float currentTime)
{
    if (currentTime > times[times.size() - 1]) __debugbreak();

    uint32_t segment = 0;
    while (currentTime > times[segment])
    {
        segment++;
    }
    
    float start = times[segment - 1];
    float end = times[segment];
    float frac = (currentTime - start) / (end - start);
    
    return { segment, frac };
}


void AnimatedMesh::CalculatePose(Animation& animation, Bone& skeletion,
                                 float time, std::vector<glm::mat4>& output,
                                 glm::mat4& parentTransform,
                                 glm::mat4& globalInverseTransform)
{
    BoneTransformTrack& btt = animation.boneTransforms[skeletion.name];
    time = fmod(time, animation.duration);
    std::pair<uint32_t, float> fp;

    //calculate interpolated position
    fp = GetTimeFraction(btt.positionTimestamps, time);
    glm::vec3 position1 = btt.positions[fp.first - 1];
    glm::vec3 position2 = btt.positions[fp.first];
    glm::vec3 position = glm::mix(position1, position2, fp.second);

    //calculate interpolated rotation
    fp = GetTimeFraction(btt.rotationTimestamps, time);
    glm::quat rotation1 = btt.rotations[fp.first - 1];
    glm::quat rotation2 = btt.rotations[fp.first];
    glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);

    //calculate interpolated scale
    fp = GetTimeFraction(btt.scaleTimestamps, time);
    glm::vec3 scale1 = btt.scales[fp.first - 1];
    glm::vec3 scale2 = btt.scales[fp.first];
    glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

    glm::mat4 positionMat = glm::mat4(1.0);
    glm::mat4 scaleMat = glm::mat4(1.0);

    // calculate localTransform
    positionMat = glm::translate(positionMat, position);
    glm::mat4 rotationMat = glm::toMat4(rotation);
    scaleMat = glm::scale(scaleMat, scale);

    glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
    glm::mat4 globalTransform = parentTransform * localTransform;

    output[skeletion.id] = globalInverseTransform * globalTransform * skeletion.offset;

    //update values for children bones
    for (Bone& child : skeletion.children) 
    {
        CalculatePose(animation, child, time, output, globalTransform, globalInverseTransform);
    }
}


} // namespace FQW