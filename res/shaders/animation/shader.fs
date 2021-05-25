#version 460 core

in vec3 v_normal;
in vec3 v_position;

out vec4 frag_color;

void main()
{
    //vec3 ambient = 0.1 * vec3(1.0);
    //vec3 light_color = vec3(1.0);
    //vec3 object_color = vec3(1.0);
    //
    //vec3 light_dir = normalize(v_position - vec3(0, 1, -1));
    //
    //float diff = max(dot(v_normal, light_dir), 0.0);
    //vec3 diffuse = diff * light_color;
    //
    //vec3 result_color = (ambient + diffuse) * object_color;
    //
    //frag_color = vec4(result_color, 1.0);
    frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}