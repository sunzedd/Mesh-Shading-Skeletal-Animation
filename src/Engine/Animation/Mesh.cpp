#include "Mesh.h"


namespace FQW {

Mesh::Mesh(
    const std::vector<Vertex>& vertexBuffer,
    const std::vector<uint32_t>& indexBuffer)
    :
    m_VertexBuffer(vertexBuffer),
    m_IndexBuffer(indexBuffer)

{
    CreateVertexArrayObject();
}


void Mesh::Draw()
{
    glcheck(glBindVertexArray(m_VAO));

    glcheck(glDrawElements(
        GL_TRIANGLES,
        m_IndexBuffer.size(),
        GL_UNSIGNED_INT, (GLvoid*)0));

    glcheck(glBindVertexArray(0));
}


void Mesh::Draw(ShaderPipeline& shader, ICamera& camera)
{
    glcheck(glBindVertexArray(m_VAO));

    glcheck(glDrawElements(
        GL_TRIANGLES,
        m_IndexBuffer.size(),
        GL_UNSIGNED_INT, (GLvoid*)0));

    glcheck(glBindVertexArray(0));
}


void Mesh::DrawWithMeshShader()
{
    glcheck(glBindVertexArray(m_VAO));
    GLuint mesh_task_count = m_IndexBuffer.size() / 3 / 32;
    glcheck(glDrawMeshTasksNV(0, mesh_task_count));
    glcheck(glBindVertexArray(0));
}


void Mesh::DrawWithMeshShader(ShaderPipeline& shader, ICamera& camera)
{
    glcheck(glBindVertexArray(m_VAO));
    GLuint mesh_task_count = m_IndexBuffer.size() / 3 / 32;
    glcheck(glDrawMeshTasksNV(0, mesh_task_count));
    glcheck(glBindVertexArray(0));
}


void Mesh::CreateVertexArrayObject()
{
    // VAO, VBO, EBO generation
    glcheck(glGenVertexArrays(1, &m_VAO));
    glcheck(glGenBuffers(1, &m_VBO));
    glcheck(glGenBuffers(1, &m_EBO));

    glcheck(glBindVertexArray(m_VAO));

    // VBO
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    glcheck(glBufferData(
        GL_ARRAY_BUFFER,
        m_VertexBuffer.size() * sizeof(Vertex),
        &(m_VertexBuffer[0]), GL_STATIC_DRAW));

    // EBO
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
    glcheck(glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_IndexBuffer.size() * sizeof(uint32_t),
        &(m_IndexBuffer[0]), GL_STATIC_DRAW));

    // Vertex attribute layout
    glcheck(glEnableVertexAttribArray(0));
    glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));

    glcheck(glEnableVertexAttribArray(1));
    glcheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, normal)));

    glcheck(glEnableVertexAttribArray(2));
    glcheck(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, boneIds)));

    glcheck(glEnableVertexAttribArray(3));
    glcheck(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, boneWeights)));

    glcheck(glBindVertexArray(0));
}



} // namespace FQW