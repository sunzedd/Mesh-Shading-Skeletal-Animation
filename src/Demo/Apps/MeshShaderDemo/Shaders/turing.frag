#version 460 core

layout(location=0) in MeshShader_Output
{
    vec3 meshletColor;
    vec3 normal;
} meshShader_Output;

out vec4 fragColor;

uniform bool u_colorize_meshlet;


void main()
{
    if (u_colorize_meshlet)
    {
        fragColor = vec4(meshShader_Output.meshletColor, 1.0);
    }
    else
    {
        fragColor = vec4(meshShader_Output.normal, 1.0);
    }
}