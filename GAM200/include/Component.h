#pragma once
/* !
@file	Component.h
@author	Yeo Jia Ming
@date	28/9/2023

The file contains declarations and implmentations for the component class,
which is a base class for other specific types of game components to derive from.
Contains a pointer to the object that the particualr game component is a part of.
*//*__________________________________________________________________________*/

#include <Debug.h>
#include <Object.h>
#include "../src/Assets Manager/serialization.h"

//different types of game components to differentiate
enum class ComponentType
{
	None = 0, //Invalid component id
	Transform,
	Texture,
	Body,
	Physics,
	PlayerControllable,
	Animation,
	Event
};

class Component
{
public:
	friend class Object;

	Component() : Base{nullptr} {}
	virtual ~Component() = default;

	//to initialise remainng game components not set from text file
	virtual void Initialize() {};

	//to allow serialization to occur
	//virtual void JsonSerialize(JsonSerialization obj) {};

	///to get the object that this component is part of
	Object* GetOwner() { return Base; }

	//to differentiate between the components
	virtual ComponentType TypeId() const { return ComponentType::None; }

private:
	///to refer to the object that component is a part of
	Object* Base;
};


