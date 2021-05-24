#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "IDrawable.h"

#include "../Animation.h"

namespace FQW {

class StaticMesh : public IDrawable
{
public:
    struct Vertex_
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoords;
    };

    StaticMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
    StaticMesh(std::vector<Vertex_> vertices, std::vector<unsigned int> indices /*TODO: textures*/);
    void Draw(Shader& shader, ICamera& camera);

private:
    void Setup();

private:
    std::vector<Vertex_> m_Vertices;
    std::vector<unsigned int> m_Indices;
    // TODO: Textures

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
};

} // namespace FQW