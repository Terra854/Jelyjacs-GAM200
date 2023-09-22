#pragma once
#include <Debug.h>

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "../../src/Assets Manager/serialization.h"

//different types of game components to differentiate
enum class ComponentType
{
	//Invalid component id
	None = 0,
	//Collision
	Transform,
	//Graphics
	Texture,
	//Physics
	Body,
};

class Component
{
public:
	friend class Object;

	//to initialise remainng game components not set from text file
	virtual void Initialize() {};

	Component() = default;
	virtual ~Component() = default;

	//for serialization to know how far to read in text file to get all the variables of this game component
	virtual void Serialize(Serialization& stream) {};

	///to get the object that this component is part of
	Object* GetOwner() { return Base; }

	//to differentiate between the components
	ComponentType TypeId;

private:
	///to refer to the object that component is a part of
	Object* Base;
};


