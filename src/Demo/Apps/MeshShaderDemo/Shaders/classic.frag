#version 460 core

in VS_Output
{
    vec3 position;
    vec3 normal;
    vec3 color;
} vs_Output;

out vec4 fragColor;

void main()
{
    fragColor = vec4(vs_Output.normal, 1.0);
}