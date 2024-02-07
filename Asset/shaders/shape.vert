/*!
@file    shape.vert
@author  Guo CHen (g.chen@digipen.edu)
@date    14/09/2023

Vertex shader shape
Transforms the vertex position from model space to normalized device coordinates (NDC).
*//*__________________________________________________________________________*/

#version 450 core

layout (location=0) in vec2 aVertexPosition;

uniform mat3 uModel_to_NDC;

void main()
{
	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)),0.0, 1.0);
}