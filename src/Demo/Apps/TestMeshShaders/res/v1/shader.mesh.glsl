#version 450
#extension GL_NV_mesh_shader : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_NV_gpu_shader5 : enable


// ����� ������� ��� ������� ������ (�������� 32)
layout(local_size_x = 32, local_size_y = 1, local_size_z = 1) in;
layout(triangles, max_vertices = 64, max_primitives = 126) out;


// ������� OpenGL
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
	uint8_t indices[126 * 3]; // up to 126 triangles
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

uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

void main()
{
    uint mi = gl_WorkGroupID.x;             // meshlet index
    uint ti = gl_LocalInvocationID.x;       // thread index
    uint vertexCount = uint(meshlets[mi].vertexCount);
    uint triangleCount = uint(meshlets[mi].triangleCount);
    uint indexCount = triangleCount * 3;
    //
    //for (uint i = ti; i < vertexCount; i+=32)
    //{
    //    uint vi = meshlets[mi].vertices[i];
    //
    //    vec4 position = vec4(vertices[vi].position.xyz, 1.0);
    //    position = view_projection_matrix * model_matrix * position;
    //    gl_MeshVerticesNV[i].gl_Position = position;
    //}
    //
    //for (uint i = ti; i < indexCount; i+=32)
    //{
    //    gl_PrimitiveIndicesNV[i] = uint(meshlets[mi].indices[i]);
    //}
    //
    //if (ti == 0)
    //{
    //    gl_PrimitiveCountNV = uint(meshlets[mi].triangleCount);
    //}

    uint base_index = ti * 3;

    uint idx0 = meshlets[mi].indices[base_index + 0];
    uint idx1 = meshlets[mi].indices[base_index + 1];
    uint idx2 = meshlets[mi].indices[base_index + 2];

    vec4 position0 = vec4(vertices[meshlets[mi].vertices[idx0]].position.xyz, 1.0);
    vec4 position1 = vec4(vertices[meshlets[mi].vertices[idx1]].position.xyz, 1.0);
    vec4 position2 = vec4(vertices[meshlets[mi].vertices[idx2]].position.xyz, 1.0);

    gl_MeshVerticesNV[base_index + 0].gl_Position = view_projection_matrix * model_matrix * position0;
    gl_MeshVerticesNV[base_index + 1].gl_Position = view_projection_matrix * model_matrix * position1;
    gl_MeshVerticesNV[base_index + 2].gl_Position = view_projection_matrix * model_matrix * position2;

    gl_PrimitiveIndicesNV[base_index + 0] = idx0;
    gl_PrimitiveIndicesNV[base_index + 1] = idx1;
    gl_PrimitiveIndicesNV[base_index + 2] = idx2;

    if (ti == 0)
    {
        gl_PrimitiveCountNV = meshlets[mi].triangleCount;
    }
}
