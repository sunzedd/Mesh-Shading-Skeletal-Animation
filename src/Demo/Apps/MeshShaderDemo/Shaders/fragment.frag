#version 460 core

in vec3 v_normal;
in vec3 v_position;
in vec3 v_color;

out vec4 frag_color;

void main()
{
    if (v_color == vec3(0.0, 0.0, 0.0))
    {
        frag_color = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        frag_color = vec4(v_color, 1.0);
    }
}