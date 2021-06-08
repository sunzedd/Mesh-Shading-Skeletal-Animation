#version 460 core
#extension GL_EXT_gpu_shader4 : require

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal;
layout (location = 2) in ivec4 boneIds;
layout (location = 3) in vec4 boneWeights;


out VS_Output
{
    vec3 position;
    vec3 normal;
    vec3 color;
} vs_Output;


uniform mat4 u_bone_transforms[50];
uniform mat4 u_M_matrix;
uniform mat4 u_VP_matrix;
uniform mat4 u_MVP_matrix;
uniform vec3 u_color = vec3(1.0, 1.0, 1.0);


out gl_PerVertex
{
    vec4 gl_Position;
};

mat4 CalculateSkinTransformationMatrix()
{
    mat4 transform = mat4(0);
    for (int i = 0; i < 4; i++)
    {
        transform += u_bone_transforms[boneIds[i]] * boneWeights[i];
    }
    return transform;
}

void main()
{    
    mat4 skinnedTransform = CalculateSkinTransformationMatrix();
   
    vec4 skinned_position = skinnedTransform * vec4(position, 1.0);   // Позиция вершины в локальном  модельном пространстве
    skinned_position = u_MVP_matrix * skinned_position;

    gl_Position = skinned_position;
    
    vs_Output.position = vec3(u_M_matrix * vec4(position, 1.0)); // позиция вершины в мировом пространстве для расчета освещения
    vs_Output.normal = mat3(transpose(inverse(u_M_matrix * skinnedTransform))) * normal;
    vs_Output.normal = normalize(vs_Output.normal);

    gl_Position = skinned_position;
}
