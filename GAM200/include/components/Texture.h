#pragma once
#include <Debug.h>

#include "Composition.h"
#include "../../src/Assets Manager/serialization.h"
#include "../Graphic_gl/glapp.h"

class Texture : public Component
{
public:
	virtual void Serialize(Serialization& stream) override;

	// Path to file for texture
	GLuint texturepath;
};