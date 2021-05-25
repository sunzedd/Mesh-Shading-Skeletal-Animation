#version 330 core

in vec3 Normal;
in vec3 FragPosition;

out vec4 o_FragColor;

void main()
{
    vec3 ambient = 0.1f * vec3(1.0f, 1.0f, 1.0f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

    vec3 n = normalize(Normal);
    vec3 lightDir = normalize(FragPosition - vec3(0, 0, -1));

    float diff = max(dot(n, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 resultColor = (ambient + diffuse) * objectColor;

    o_FragColor = vec4(resultColor, 1.0f);
}
