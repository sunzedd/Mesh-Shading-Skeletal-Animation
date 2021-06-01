#version 460 core
#extension GL_EXT_gpu_shader4 : require

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal;
layout (location = 2) in ivec4 boneIds;
layout (location = 3) in vec4 boneWeights;

out vec3 v_normal;
out vec3 v_position;
out vec3 v_color;

uniform mat4 bone_transforms[50];
uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;


out gl_PerVertex
{
    vec4 gl_Position;
};

mat4 CalculateSkinTransformationMatrix()
{
    mat4 transform = mat4(0);
    for (int i = 0; i < 4; i++)
    {
        transform += bone_transforms[boneIds[i]] * boneWeights[i];
    }
    return transform;
}

void main()
{    
    //vec4 positions[] = {
    //    {-1.0, -1.0, 0.0, 1.0},
    //    {0.0, 1.0, 0.0, 1.0},
    //    {1.0, -1.0, 0.0, 1.0},
    //};

    mat4 skinnedTransform = CalculateSkinTransformationMatrix();
    vec4 skinned_position = skinnedTransform * vec4(position, 1.0);   // Позиция вершины в локальном  модельном пространстве

    vec4 final_position = view_projection_matrix * model_matrix * skinned_position;
    //if (final_position == vec4(0, 0, 0, 0))
    //{
    //    v_color = vec3(1, 1, 1);
    //    final_position = view_projection_matrix * model_matrix * vec4(position, 1.0);
    //}

    gl_Position = final_position;
    //gl_Position = vec4(0, 0, 0, 1);

    v_position = vec3(model_matrix * vec4(position, 1.0)); // позиция вершины в мировом пространстве для расчета освещения
    v_normal = mat3(transpose(inverse(model_matrix * skinnedTransform))) * normal;
    v_normal = normalize(v_normal);  //v_normal = normal;

    //gl_Position = view_projection_matrix * model_matrix * vec4(position, 1.0);
    //gl_Position = positions[ int(mod(gl_VertexID, 3)) ];

}
