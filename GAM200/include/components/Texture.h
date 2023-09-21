#pragma once
#include <Debug.h>

#include "Composition.h"
#include "../../src/Assets Manager/serialization.h"
#include "../Graphic_gl/glapp.h"

class Texture : public GameComponent
{
	public:

	GLuint texturepath;

	virtual void Serialize(Serialization& stream) override;

};