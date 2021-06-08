#include "Mesh.h"


namespace FQW {


Mesh::Mesh(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer, IMeshletBuilder& meshletBuilder)
    :
    m_VertexBuffer(vertexBuffer),
    m_IndexBuffer(indexBuffer)
{
    CreateBuffersForClassicPipeline();

    m_Meshlets = meshletBuilder.Build(vertexBuffer, indexBuffer);
    CreateBuffersForTuringPipeline();
}


void Mesh::Draw(ShaderPipeline& shaderPipeline, ICamera& camera)
{
    switch (shaderPipeline.GetType())
    {
        case ShaderPipeline::Type::Classic:     DrawWithClassicPipeline(shaderPipeline, camera); break;
        case ShaderPipeline::Type::TuringRTX:   DrawWithTuringPipeline(shaderPipeline, camera);  break;

        default: FQW_CRITICAL("Unknown render pipeline type passed"); break;
    }
}


void Mesh::DrawWithClassicPipeline(ShaderPipeline& shaderPipeline, ICamera& camera)
{
    shaderPipeline.Use();
    glcheck(glBindVertexArray(m_ClassicPipelineBuffers.VAO));
    glcheck(glDrawElements(GL_TRIANGLES, m_IndexBuffer.size(), GL_UNSIGNED_INT, (GLvoid*)0) );
    glcheck(glBindVertexArray(0));
}


void Mesh::DrawWithTuringPipeline(ShaderPipeline& shaderPipeline, ICamera& camera)
{
    shaderPipeline.Use();
    glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_TuringPipelineBuffers.vertices_SSBO));
    glcheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_TuringPipelineBuffers.meshlets_SSBO));
    GLuint workgroupCount = m_Meshlets.size();
    glcheck(glDrawMeshTasksNV(0, workgroupCount));
}


void Mesh::CreateBuffersForClassicPipeline()
{
    // VAO, VBO, EBO generation
    glcheck(glGenVertexArrays(1, &m_ClassicPipelineBuffers.VAO));
    glcheck(glGenBuffers(1, &m_ClassicPipelineBuffers.VBO));
    glcheck(glGenBuffers(1, &m_ClassicPipelineBuffers.EBO));

    glcheck(glBindVertexArray(m_ClassicPipelineBuffers.VAO));

    // VBO
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_ClassicPipelineBuffers.VBO));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_VertexBuffer.size() * sizeof(Vertex), &(m_VertexBuffer[0]), GL_STATIC_DRAW));

    // EBO
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ClassicPipelineBuffers.EBO));
    glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                         m_IndexBuffer.size() * sizeof(uint32_t),
                         &(m_IndexBuffer[0]), GL_STATIC_DRAW));

    // Vertex attribute layout
    glcheck(glEnableVertexAttribArray(0));
    glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));

    glcheck(glEnableVertexAttribArray(1));
    glcheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)) );

    glcheck(glEnableVertexAttribArray(2));
    glcheck(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, boneIds)) );

    glcheck(glEnableVertexAttribArray(3));
    glcheck(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)offsetof(Vertex, boneWeights)) );

    glcheck(glBindVertexArray(0));
}


void Mesh::CreateBuffersForTuringPipeline()
{
    if (m_Meshlets.size() == 0)
    {
        FQW_ERROR("No meshlets build for the mesh");
        throw std::runtime_error("No meshlets build for the mesh");
    }

    // Vertex SSBO
    glcheck(glGenBuffers(1, &m_TuringPipelineBuffers.vertices_SSBO));
    glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TuringPipelineBuffers.vertices_SSBO));

    glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER,
                         m_VertexBuffer.size() * sizeof(m_VertexBuffer[0]),
                         m_VertexBuffer.data(), 
                         GL_STATIC_DRAW));

    glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

    
    // Meshlets SSBO
    glcheck(glGenBuffers(1, &m_TuringPipelineBuffers.meshlets_SSBO));
    glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TuringPipelineBuffers.meshlets_SSBO));

    glcheck(glBufferData(GL_SHADER_STORAGE_BUFFER,
                         m_Meshlets.size() * sizeof(m_Meshlets[0]),
                         m_Meshlets.data(),
                         GL_STATIC_DRAW));

    glcheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}


} // namespace FQW