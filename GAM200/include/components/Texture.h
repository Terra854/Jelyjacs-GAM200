#pragma once
#include <Debug.h>

#include "Object.h"
#include "../../src/Assets Manager/serialization.h"
#include "../Graphic_gl/glapp.h"

class Texture : public Component
{
public:
	Texture(unsigned int _texturepath) : Component(), texturepath(_texturepath){}

	virtual void Serialize(Serialization& stream) override;

	virtual ComponentType TypeId() const override { return ComponentType::Texture; }

	// Path to file for texture
	unsigned int texturepath;
};