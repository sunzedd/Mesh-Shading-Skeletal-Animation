#version 450
#extension GL_NV_mesh_shader : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_NV_gpu_shader5 : enable

#define MAX_VER  96
#define MAX_PRIM 32

// Число потоков для рабочей группы (максимум 32)
layout(local_size_x=32) in;

// Тип примитивов
layout(triangles) out;

// Максимальные размеры мешлета
layout(max_vertices=MAX_VER) out;
layout(max_primitives=MAX_PRIM) out;

// Приколы OpenGL
out gl_PerVertex
{
    vec4 gl_Position;
  //float gl_PointSize;
  //float gl_ClipDistance[];
  //float gl_CullDistance[];
} gl_stupid;


out gl_MeshPerVertexNV 
{
    vec4 gl_Position;  
  //float gl_PointSize;
  //float gl_ClipDistance[];
  //float gl_CullDistance[];
} gl_MeshVerticesNV[MAX_VER];


// Тут атрибуты вершин
out perVertexData 
{
    vec3 color;
} customData[MAX_VER];



uniform mat4 mvp;


struct Meshlet
{
	uint vertices[64];
	uint8_t indices[126]; // up to 42 triangles
	uint8_t indexCount;
	uint8_t vertexCount;
};

struct Vertex_
{
    vec4 position;
    vec4 normal;
    vec4 boneIds;
    vec4 boneWeights;
};


layout(binding = 0, std430) readonly buffer Vertices
{
    Vertex_ vertices[];
};

layout(binding = 1, std430) readonly buffer Indices
{
    uint32_t indices[];
};

layout(binding = 2, std430) readonly buffer Meshlets
{
	Meshlet meshlets[];
};


void main()
{
    uint lid = gl_LocalInvocationID.x;
    uint gid = gl_GlobalInvocationID.x;

    uint base_index = gid * 3;
    uvec3 face_indices = uvec3(indices[base_index], indices[base_index + 1], indices[base_index + 2]);
    uint vertex_index = lid * 3;

    gl_MeshVerticesNV[vertex_index + 0].gl_Position = mvp * vertices[face_indices.x].position;
    gl_MeshVerticesNV[vertex_index + 1].gl_Position = mvp * vertices[face_indices.y].position;
    gl_MeshVerticesNV[vertex_index + 2].gl_Position = mvp * vertices[face_indices.z].position;

    gl_PrimitiveIndicesNV[vertex_index] = vertex_index;
    gl_PrimitiveIndicesNV[vertex_index + 1] = vertex_index + 1;
    gl_PrimitiveIndicesNV[vertex_index + 2] = vertex_index + 2;

    if (gl_LocalInvocationID.x == 0) {
        gl_PrimitiveCountNV = 32;
    }
}