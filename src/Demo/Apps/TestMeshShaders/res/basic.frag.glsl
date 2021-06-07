#version 450
layout(location = 0) in vec3 color;
out vec4 OutColor;

void main()
{
    OutColor = vec4(color.xyz, 1.0);
}
