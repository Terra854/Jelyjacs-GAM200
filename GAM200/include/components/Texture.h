#pragma once
#include <Debug.h>

#include "Composition.h"
#include "../../src/Assets Manager/serialization.h"
#include "../Graphic_gl/glapp.h"

class Texture : public GameComponent
{
	public:

	GLuint texturepath;

<<<<<<< HEAD
	virtual void Serialize(Serialization& stream) override;
=======
	virtual void Serialize(Serialization& stream) override
	{
		std::string texturefile;
		streamGet(stream, texturefile);
		texturefile = "../../../Asset/Picture/" + texturefile;
		texturepath = GLApp::setup_texobj(texturefile.c_str());
	}
>>>>>>> a5765f1284347935d7736f4e3bbd78ecb01ebfb6

};