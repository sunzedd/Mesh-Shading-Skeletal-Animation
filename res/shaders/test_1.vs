#version 330 core

layout (location=0) in vec3 a_Position;
layout (location=1) in vec3 a_Normal;
layout (location=2) in vec2 a_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_ModelMatrix;

out vec3 Normal;
out vec3 FragPosition;

void main() 
{
	gl_Position = u_MVP * vec4(a_Position, 1.0f);

	FragPosition = vec3(u_ModelMatrix * vec4(a_Position, 1.0f));
	Normal = a_Normal;
}
