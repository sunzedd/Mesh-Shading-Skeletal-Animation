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
    OPENGL_CALL(glBindVertexArray(m_VAO));

    OPENGL_CALL(glDrawElements(
        GL_TRIANGLES,
        m_IndexBuffer.size(),
        GL_UNSIGNED_INT, (GLvoid*)0));

    OPENGL_CALL(glBindVertexArray(0));
}


void Mesh::Draw(Shader& shader, ICamera& camera)
{
    OPENGL_CALL(glBindVertexArray(m_VAO));

    OPENGL_CALL(glDrawElements(
        GL_TRIANGLES,
        m_IndexBuffer.size(),
        GL_UNSIGNED_INT, (GLvoid*)0));

    OPENGL_CALL(glBindVertexArray(0));
}


void Mesh::CreateVertexArrayObject()
{
    // VAO, VBO, EBO generation
    OPENGL_CALL(glGenVertexArrays(1, &m_VAO));
    OPENGL_CALL(glGenBuffers(1, &m_VBO));
    OPENGL_CALL(glGenBuffers(1, &m_EBO));

    OPENGL_CALL(glBindVertexArray(m_VAO));

    // VBO
    OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    OPENGL_CALL(glBufferData(
        GL_ARRAY_BUFFER,
        m_VertexBuffer.size() * sizeof(Vertex),
        &(m_VertexBuffer[0]), GL_STATIC_DRAW));

    // EBO
    OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
    OPENGL_CALL(glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
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



} // namespace FQW