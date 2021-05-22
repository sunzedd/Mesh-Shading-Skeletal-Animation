#include "StaticMesh.h"

namespace FQW {

StaticMesh::StaticMesh(std::vector<StaticMesh::Vertex> vertices, std::vector<unsigned int> indices /*TODO: textures*/)
{
    m_Vertices = vertices;
    m_Indices = indices;

    Setup();
}

void StaticMesh::Setup()
{
    // Buffers
    OPENGL_CALL( glGenVertexArrays(1, &m_VAO) );
    OPENGL_CALL( glGenBuffers(1, &m_VBO) );
    OPENGL_CALL( glGenBuffers(1, &m_EBO) );

    OPENGL_CALL( glBindVertexArray(m_VAO) );

    OPENGL_CALL( glBindBuffer(GL_ARRAY_BUFFER, m_VBO) );
    OPENGL_CALL( glBufferData(GL_ARRAY_BUFFER,
                              m_Vertices.size() * sizeof(Vertex), &(m_Vertices[0]), GL_STATIC_DRAW) );

    OPENGL_CALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO) );
    OPENGL_CALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                              m_Indices.size() * sizeof(unsigned int), &(m_Indices[0]), GL_STATIC_DRAW) );

    // Vertex attribute layout
    OPENGL_CALL( glEnableVertexAttribArray(0) );
    OPENGL_CALL( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0) );

    OPENGL_CALL(glEnableVertexAttribArray(1));
    OPENGL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (void*)offsetof(Vertex, normal)) );

    OPENGL_CALL(glEnableVertexAttribArray(2));
    OPENGL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (void*)offsetof(Vertex, textureCoords)) );
}


void StaticMesh::Draw(Shader& shader, ICamera& camera)
{
    OPENGL_CALL( glBindVertexArray(m_VAO) );
    OPENGL_CALL( glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0) );
    OPENGL_CALL( glBindVertexArray(0) );
}

} // namespace FQW