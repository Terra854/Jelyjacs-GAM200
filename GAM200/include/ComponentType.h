#pragma once
#include <Debug.h>

//different types of game components to differentiate
	enum class ComponentTypeId
	{
		//Invalid component id
		CT_None = 0,
		//Collision
		CT_Transform,
		//Graphics
		CT_Texture,
		//Physics
		CT_Body,
		//Max
		CT_MaxComponents
	};