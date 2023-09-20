#pragma once

#include "Composition.h"
#include "../../src/Assets Manager/serialization.h"

class Texture : public GameComponent
{
	public:
		std::string texturepath;

	virtual void Serialize(Serialization& stream) override
	{
		streamGet(stream, texturepath);
	}
};