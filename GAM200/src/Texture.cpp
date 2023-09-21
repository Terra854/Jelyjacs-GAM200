#pragma once
#include "../include/components/Texture.h"

/****************************************************************
* Serialize Function
* - Get texture file path
*****************************************************************/
void Texture::Serialize(Serialization& stream) 
{
	std::string texturefile;
	streamGet(stream, texturefile);
	texturefile = "../../../Asset/Picture/" + texturefile;
	texturepath = app->setup_texobj(texturefile.c_str());
}