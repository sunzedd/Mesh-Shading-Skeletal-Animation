#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "IDrawable.h"

namespace FQW {

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};


class Mesh : public IDrawable
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices /*TODO: textures*/);
    void Draw(Shader& shader, ICamera& camera);

private:
    void Setup();

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    // TODO: Textures

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
};

} // namespace FQW