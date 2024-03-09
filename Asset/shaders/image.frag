/*!
@file    image.frag
@author  Guo CHen (g.chen@digipen.edu)
@date    14/09/2023

Fragment shader image
Texture handler will be passed in as a uniform variable
*//*__________________________________________________________________________*/

#version 450 core

layout (location=1) in vec2 vTexCoord;
layout (location=0) out vec4 fFragColor;



uniform sampler2D uTex2d;
uniform float uOpacity;
void main () 
{

        fFragColor = texture(uTex2d, vTexCoord);

        // if uopacity has been set, then use it
        if(uOpacity<1.0)
        {
            fFragColor.a = uOpacity * fFragColor.a;
        }
       
}