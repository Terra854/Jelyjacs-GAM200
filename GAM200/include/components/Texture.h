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
	Texture(unsigned int _texturepath) : Component(), texturepath(_texturepath){}

	virtual ComponentType TypeId() const override { return ComponentType::Texture; }

	// Path to file for texture
	unsigned int texturepath;
};