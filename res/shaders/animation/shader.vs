#version 460 core

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal;
layout (location = 2) in ivec4 boneIds;
layout (location = 3) in vec4 boneWeights;

out vec3 v_normal;
out vec3 v_position;

uniform mat4 bone_transforms[50];
uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

void main()
{
    vec4 bw = vec4(0);
    mat4 bone_transform = mat4(0.0);

    bone_transform += bone_transforms[boneIds.x] * boneWeights.x;    
    bone_transform += bone_transforms[boneIds.y] * boneWeights.y;    
    bone_transform += bone_transforms[boneIds.z] * boneWeights.z;    
    bone_transform += bone_transforms[boneIds.w] * boneWeights.w;    

    vec4 position = bone_transform * vec4(position, 1.0);   // Позиция вершины в локальном  модельном пространстве

    gl_Position = view_projection_matrix * model_matrix * position;

    v_position = vec3(model_matrix * position); // позиция вершины в мировом пространстве для расчета освещения

    v_normal = mat3(transpose(inverse(model_matrix * bone_transform))) * normal;
    v_normal = normalize(v_normal);
}
