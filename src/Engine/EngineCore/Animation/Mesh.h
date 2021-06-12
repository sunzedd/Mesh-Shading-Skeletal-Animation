#pragma once
#include "../../Utility/AssimpGlmConverter.h"

#include "../../MeshShading/IMeshletBuilder.h"

#include "../Core/Math.h"
#include "../Core/Logger.h"

#include "../Graphics/IDrawable.h"
#include "../Graphics/OpenGL_Utils.h"

#include "Vertex.h"


namespace FQW {


class Mesh : public IDrawable
{
public:
    Mesh(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer, std::weak_ptr<IMeshletBuilder> meshletBuilder);

    void Draw(ShaderPipeline& shader, ICamera& camera) override;

    const vector<Vertex>& GetVertexBuffer() const { return m_VertexBuffer; }
    const vector<uint32_t>& GetIndexBuffer() const { return m_IndexBuffer; }
    uint32_t GetVertexArrayObject() const { return m_ClassicPipelineBuffers.VAO; }

private:
    void DrawWithClassicPipeline(ShaderPipeline& shaderPipeline, ICamera& camera);
    void DrawWithTuringMeshShaderPipeline(ShaderPipeline& shaderPipeline, ICamera& camera);
    void DrawWithTuringTaskMeshShaderPipeline(ShaderPipeline& shaderPipeline, ICamera& camera);

    void CreateBuffersForClassicPipeline();
    void CreateBuffersForTuringPipeline();

private:
    const vector<Vertex> m_VertexBuffer;
    const vector<uint32_t> m_IndexBuffer;

    struct {
        uint32_t VBO = 0;
        uint32_t EBO = 0;
        uint32_t VAO = 0;
    } m_ClassicPipelineBuffers;

    struct {
        uint32_t vertices_SSBO;
        uint32_t meshlets_SSBO;
    } m_TuringPipelineBuffers;

    vector<Meshlet> m_Meshlets;
};

} // namespace FQW