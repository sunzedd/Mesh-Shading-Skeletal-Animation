#pragma once
#include "../EngineCore/Core/Math.h"

#include "../EngineCore/Core/Logger.h"
#include "../EngineCore/Graphics/IDrawable.h"
#include "../EngineCore/Graphics/OpenGL_Utils.h"
#include "../Utility/AssimpGlmConverter.h"



namespace FQW {

struct Vertex
{
    vec3  position;
    vec3  normal;
    ivec4 boneIds = ivec4(0);
    vec4  boneWeights = vec4(0.0f);
};


class Mesh : public IDrawable
{
public:
    Mesh(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer);

    void Draw();
    void Draw(ShaderPipeline& shader, ICamera& camera) override;

    void DrawWithMeshShader();
    void DrawWithMeshShader(ShaderPipeline& shader, ICamera& camera) override;

    const vector<Vertex>& GetVertexBuffer() const { return m_VertexBuffer; }
    const vector<uint32_t>& GetIndexBuffer() const { return m_IndexBuffer; }
    uint32_t GetVertexArrayObject() const { return m_VAO; }

private:
    const vector<Vertex> m_VertexBuffer;
    const vector<uint32_t> m_IndexBuffer;

    uint32_t m_VBO = 0;
    uint32_t m_EBO = 0;
    uint32_t m_VAO = 0;

    void CreateVertexArrayObject();
};

} // namespace FQW