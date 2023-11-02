#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in vec2 aTextureCoordinate;

layout (location=0) out vec3 vColor;
layout (location=1) out vec2 vTexCoord;
uniform mat3 uModel_to_NDC;
void main()
{
    vColor = aColor;
    vTexCoord = aTextureCoordinate;
    gl_Position = vec4(vec2(uModel_to_NDC *vec3(aPos + aOffset,1.0)), 0.0, 1.0);
}