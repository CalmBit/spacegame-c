#version 150

in vec3 vPos;
out vec3 Pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Pos = vPos;
    gl_Position = proj * view * model * vec4(vPos, 1.0);
}
