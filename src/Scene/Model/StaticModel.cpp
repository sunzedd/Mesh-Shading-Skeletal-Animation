#include "StaticModel.h"

namespace FQW {

StaticModel::StaticModel(std::string filepath)
{
    Load(filepath);
    FQW_TRACE("[Assimp] Succesfuly loaded model from {}", filepath);
}


void StaticModel::Draw(Shader& shader, ICamera& camera)
{
    glm::mat4 S = glm::mat4(1.0f);      // Scale
    glm::mat4 Rx = glm::mat4(1.0f);     // Rotate X
    glm::mat4 Ry = glm::mat4(1.0f);     // Rotate Y
    glm::mat4 Rz = glm::mat4(1.0f);     // Rotate Z
    glm::mat4 R = glm::mat4(1.0f);
    glm::mat4 T = glm::mat4(1.0f);      // Translate

    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 V = glm::mat4(1.0f);
    glm::mat4 P = glm::mat4(1.0f);
    glm::mat4 MVP = glm::mat4(1.0f);

    S = glm::scale(S, glm::vec3(1, 1, 1));
    Rx = glm::rotate(Rx, 0.0f, glm::vec3(1, 0, 0));
    Ry = glm::rotate(Ry, 0.0f, glm::vec3(0, 1, 0));
    Rz = glm::rotate(Rz, 0.0f, glm::vec3(0, 0, 1));
    R = Rx * Ry * Rz;
    T = glm::translate(T, glm::vec3(0, 0, 0));

    M = T * R * S;
    V = camera.GetViewMatrix();
    P = camera.GetProjectionMatrix();
    MVP = P * V * M;

    shader.Use();
    shader.SetMatrix4fv("u_ModelMatrix", M);
    shader.SetMatrix4fv("u_MVP", MVP);

    for (Ref<IDrawable> mesh : m_Meshes)
    {
        mesh->Draw(shader, camera);
    }
}


void StaticModel::Load(std::string filepath)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        FQW_ERROR("[Assimp] Failed to load model from {}.\nError: {}", filepath, import.GetErrorString());
        throw std::runtime_error("[Assimp] Failed to load model from " + filepath);
    }

    m_Directory = filepath.substr(0, filepath.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}


void StaticModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}


Ref<StaticMesh> StaticModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<StaticMesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    // TODO: Textures

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        StaticMesh::Vertex vertex;

        const aiVector3D& aiPosition = mesh->mVertices[i];
        vertex.position.x = aiPosition.x;
        vertex.position.y = aiPosition.y;
        vertex.position.z = aiPosition.z;

        if (mesh->HasNormals())
        {
            const aiVector3D& aiNormal = mesh->mNormals[i];
            vertex.normal.x = aiNormal.x;
            vertex.normal.y = aiNormal.y;
            vertex.normal.z = aiNormal.z;
        }

        if (mesh->mTextureCoords[0])
        {
            // TODO: Textures
        }

        // TEMP:
        vertex.textureCoords = glm::vec2(0, 0);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return CreateRef<StaticMesh>(vertices, indices);
}

} // namespace FQW