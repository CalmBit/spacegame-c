#version 150

in vec2 vPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(vPos, 0.0, 1.0);
}
