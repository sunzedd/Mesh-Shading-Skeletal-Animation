#version 110

varying vec3 color;
uniform float u_intensity;

void main()
{
    gl_FragColor = u_intensity * vec4(color, 1.0);
}