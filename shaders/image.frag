/*!
@file    my-tutorial-5.frag
@author  pghali@digipen.edu, g.chen@digipen.edu
@date    10/06/2023

Fragment shader for tutorial 4.
The shader performs different operations based on the value of uShaderMode. 
*//*__________________________________________________________________________*/
#version 450 core

layout (location=1) in vec2 vTexCoord;
layout (location=0) in vec3 vInterpColor;
layout (location=0) out vec4 fFragColor;



uniform sampler2D uTex2d;

void main () 
{
    
    
        fFragColor = texture(uTex2d, vTexCoord);
    
        //fFragColor = vec4 (vInterpColor.x * fFragColor.x ,vInterpColor.y * fFragColor.y,vInterpColor.z * fFragColor.z, fFragColor.a );
    
}