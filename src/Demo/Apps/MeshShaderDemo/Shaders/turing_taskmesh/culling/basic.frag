#version 460 core

layout(location=0) in MeshShader_Output
{
    flat uint meshletClassID;
    vec3 meshletColor;
    vec3 normal;
} meshShader_Output;

out vec4 fragColor;

uniform bool u_colorize_meshlet;
uniform bool u_show_meshlet_class_groups;


void main()
{
    if (u_colorize_meshlet)
    {
        if (u_show_meshlet_class_groups)
        {
            if (meshShader_Output.meshletClassID == 0)      // alpha RED
                fragColor = vec4(0.8, 0.25, 0.25, 1.0);
            else if (meshShader_Output.meshletClassID == 1) // betta BLUE
                fragColor = vec4(0.3, 0.3, 0.7, 1.0);
            else if (meshShader_Output.meshletClassID == 2) // omega GREEN
                fragColor = vec4(0.2, 0.8, 0.4, 1.0);
        }
        else
            fragColor = vec4(meshShader_Output.meshletColor, 1.0);
    }
    else
    {
        fragColor = vec4(meshShader_Output.normal, 1.0);
    }
}