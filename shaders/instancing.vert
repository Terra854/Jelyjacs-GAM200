#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;
uniform mat3 uModel_to_NDC;
void main()
{
    fColor = aColor;
    gl_Position = vec4(vec2(uModel_to_NDC *vec3(aPos + aOffset,1.0)), 0.0, 1.0);
}