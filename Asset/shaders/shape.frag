/*!
@file    shape.frag
@author  Guo CHen (g.chen@digipen.edu)
@date    14/09/2023

This file contains the fragment shader for the shape program.
color is passed in as a uniform
*//*__________________________________________________________________________*/

#version 450 core

uniform vec3 uColor;

layout (location=0) out vec4 fFragColor;

void main()
{
	fFragColor = vec4(uColor, 1.0);
}