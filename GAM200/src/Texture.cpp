/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
#include "../include/components/Texture.h"

/****************************************************************
* Serialize Function
* - Get texture file path
*****************************************************************/
void Texture::Serialize(Serialization& stream) 
{
	if (stream.isGood())
	{
		std::string texturefile;
		streamGet(stream, texturefile);
		texturefile = "../Asset/Picture/" + texturefile;
		std::cout << "Texture file location: " << texturefile << std::endl;
		texturepath = app->setup_texobj(texturefile.c_str());
	}
	else
	{
		std::cout << "Error serializing texture from stream" << std::endl;
	}
}