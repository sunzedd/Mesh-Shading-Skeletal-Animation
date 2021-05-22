#version 330 core

layout (location=0) in vec3 a_Position;
layout (location=1) in vec3 a_Normal;
layout (location=2) in vec2 a_TexCoord;

uniform mat4 u_MVP;
out vec4 color;

void main() 
{
	gl_Position = u_MVP * vec4(a_Position, 1.0f);
	color = gl_Position; //vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
