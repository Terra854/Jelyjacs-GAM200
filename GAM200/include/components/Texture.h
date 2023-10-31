#pragma once
/* !
@file	Texture.h
@author	Yeo Jia Ming
@date	28/9/2023

This file contains declarations and implementations for the texture component class.
Contains an id that is used in graphics to determine which texture to use for this object.
*//*__________________________________________________________________________*/
#include <Debug.h>

#include "Object.h"
#include "../../src/Assets Manager/serialization.h"
#include "../Graphic_gl/glapp.h"

class Texture : public Component
{
public:
	Texture(std::string _textureName) : Component(), textureName(_textureName){}

	virtual ComponentType TypeId() const override { return ComponentType::Texture; }

	// Path to file for texture
	std::string textureName;
};