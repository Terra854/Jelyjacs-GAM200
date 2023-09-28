#pragma once
/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
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


	///to get the object that this component is part of
	Object* GetOwner() { return Base; }

	//to differentiate between the components
	virtual ComponentType TypeId() const { return ComponentType::None; }

private:
	///to refer to the object that component is a part of
	Object* Base;
};


