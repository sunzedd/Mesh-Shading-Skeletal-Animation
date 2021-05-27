#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../EngineCore/Core/Logger.h"
#include "../EngineCore/Graphics/IDrawable.h"
#include "../EngineCore/Graphics/OpenGL_Utils.h"
#include "../EngineCore/Utility/AssimpGlmConverter.h"



namespace FQW {

struct Vertex
{
    glm::vec3  position;
    glm::vec3  normal;
    glm::ivec4 boneIds = glm::ivec4(-1);
    glm::vec4  boneWeights = glm::vec4(0.0f);

    void AddBoneData(uint32_t boneId, float weight)
    {
        for (int i = 0; i < 4; i++)
        {
            if (boneWeights[i] == 0.0f)
            {
                boneIds[i] = boneId;
                boneWeights[i] = weight;
                return;
            }
        }
    }
};


class Mesh : public IDrawable
{
public:
    Mesh(const std::vector<Vertex>& vertexBuffer,
         const std::vector<uint32_t>& indexBuffer);

    void Draw();
    void Draw(Shader& shader, ICamera& camera) override;


    const std::vector<Vertex>& GetVertexBuffer() const { return m_VertexBuffer; }
    const std::vector<uint32_t>& GetIndexBuffer() const { return m_IndexBuffer; }
    uint32_t GetVertexArrayObject() const { return m_VAO; }

private:
    const std::vector<Vertex> m_VertexBuffer;
    const std::vector<uint32_t> m_IndexBuffer;

    uint32_t m_VBO = 0;
    uint32_t m_EBO = 0;
    uint32_t m_VAO = 0;

    void CreateVertexArrayObject();
};

} // namespace FQW