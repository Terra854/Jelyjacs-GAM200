/*!
@file    my-tutorial-5.vert
@author  pghali@digipen.edu, g.chen@digipen.edu
@date    10/06/2023

Vertex shader for tutorial 4.
Transforms the vertex position from model space to normalized device coordinates (NDC).
*//*__________________________________________________________________________*/
#version 450 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec3 aVertexColor;
layout (location=2) in vec2 aTextureCoordinate;
layout (location=0) out vec3 vColor;
layout (location=1) out vec2 vTexCoord;

uniform mat3 uModel_to_NDC;

void main() 
{
	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)),0.0, 1.0);
	gl_Position = vec4(aVertexPosition,0.0, 1.0);
	vColor = aVertexColor;
	vTexCoord = aTextureCoordinate;
}