#version 640

#extension GL_NV_mesh_shader : require


layout(local_size_x = 32, local_size_y = 1, local_size_z = 1) in;  // x - count of workgroups? (WARPs)
layout(triangles, max_vertices = 64, max_primitives = 42) out;


struct Meshlet
{
    uint vertices[64];  // vertex indices in mesh vertexbuffer
    uint8_t indices[126];   // vertex indices in meshlet's vertexbuffer (Meshlet.vertices)
    uint8_t indexCount;
    uint8_t vertexCount;
};


void main()
{
    
}