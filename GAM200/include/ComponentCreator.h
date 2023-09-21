#pragma once
#include <Debug.h>

#include "component.h"
#include "ComponentType.h"

class ComponentCreator
{
public:

	//ctor to set what type of component to create
	ComponentCreator(ComponentTypeId);

	virtual ~ComponentCreator() {};

	//to new a component based on the type set by ctor
	virtual GameComponent* Create() = 0;

	//to store type of component to call create() 
	ComponentTypeId typeId;

};

//used by addcomponent function, second argument, that specifies what type of game component to create
template<typename type>
class ComponentCreatorType : public ComponentCreator
{
public:
	ComponentCreatorType(ComponentTypeId ID)
		:ComponentCreator(ID)
	{}
	virtual GameComponent* Create()
	{
		return new type();
	}
};