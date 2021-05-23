#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Graphics/StaticMesh.h"


namespace FQW {

class StaticModel : public IDrawable, public ScriptableEntity
{
public:
    StaticModel(std::string filepath);

    void Draw(Shader& shader, ICamera& camera) override;

private:

    void Load(std::string filepath);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Ref<StaticMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
    std::vector<Ref<StaticMesh>> m_Meshes;
    std::string m_Directory;
};

} // namespace FQW