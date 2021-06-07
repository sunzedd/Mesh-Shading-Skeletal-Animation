#version 460 core

layout(location = 0) in vec3 position;

out vec3 color;

uniform vec3 u_color;
uniform mat4 u_mvp = mat4(1.0);

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
	color = vec3(1.0, 0.5, 0.5);//u_color;
	gl_Position = u_mvp * vec4(position, 1.0);
}
