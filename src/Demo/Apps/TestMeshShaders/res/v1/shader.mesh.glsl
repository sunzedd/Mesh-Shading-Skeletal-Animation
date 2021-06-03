#version 450
#extension GL_NV_mesh_shader : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_NV_gpu_shader5 : enable


// Число потоков для рабочей группы (максимум 32)
layout(local_size_x = 32, local_size_y = 1, local_size_z = 1) in;
layout(triangles, max_vertices = 64, max_primitives = 126) out;


// Приколы OpenGL
out gl_PerVertex
{
    vec4 gl_Position;
} gl_stupid;

out gl_MeshPerVertexNV
{
    vec4 gl_Position;
} gl_MeshVerticesNV[];

struct Vertex_
{
    vec4 position;
    vec4 normal;
    vec4 boneIds;
    vec4 boneWeights;
};

struct Meshlet
{
	uint    vertices[64];
	uint8_t indices[126 * 3]; // up to 42 triangles
	uint8_t triangleCount;
	uint8_t vertexCount;
};


layout(binding = 0, std430) readonly buffer vertices_ssbo
{
    Vertex_ vertices[];
};

layout(binding = 1, std430) readonly buffer indices_ssbo
{
    uint32_t indices[];
};

layout(binding = 2, std430) readonly buffer meshlets_ssbo
{
	Meshlet meshlets[];
};


uniform mat4 u_mvp;

void main()
{
    uint mi = gl_WorkGroupID.x;             // meshlet  index?
    uint ti = gl_LocalInvocationID.x;       // triangle index?

    uint vertexCount = uint(meshlets[mi].vertexCount);
	uint triangleCount = uint(meshlets[mi].triangleCount);
	uint indexCount = triangleCount * 3;

    for (uint i = ti; i < vertexCount; i += 32)
    {
        uint vi = meshlets[mi].vertices[i];

        vec4 position = vec4(vertices[vi].position.xyz, 1.0);
        position = u_mvp * position;
        gl_MeshVerticesNV[i].gl_Position = position;
    }

    for (uint i = ti; i < indexCount; i += 32)
    {
        gl_PrimitiveIndicesNV[i] = uint(meshlets[mi].indices[i]);
    }
}