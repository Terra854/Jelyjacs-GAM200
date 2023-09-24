#pragma once
#include <Debug.h>
#include "../../src/Assets Manager/serialization.h"
#include <Object.h>

//different types of game components to differentiate
enum class ComponentType
{
	None = 0, //Invalid component id
	Transform,
	Texture,
	Body,
	Physics,
	PlayerControllable
};

class Component
{
public:
	friend class Object;

	Component() : Base{nullptr} {}
	virtual ~Component() = default;

	//to initialise remainng game components not set from text file
	virtual void Initialize() {};

	//for serialization to know how far to read in text file to get all the variables of this game component
	virtual void Serialize(Serialization& stream) {};

	///to get the object that this component is part of
	Object* GetOwner() { return Base; }

	//to differentiate between the components
	virtual ComponentType TypeId() const { return ComponentType::None; }

private:
	///to refer to the object that component is a part of
	Object* Base;
};


