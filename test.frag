#version 150

in vec3 Pos;
out vec4 fragColor;
void main()
{
    fragColor = vec4(Pos*2, 1.0);
}
