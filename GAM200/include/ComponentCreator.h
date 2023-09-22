#pragma once
#include <Debug.h>

#include "component.h"

class ComponentCreator
{
public:

	//ctor to set what type of component to create
	ComponentCreator(ComponentType);

	virtual ~ComponentCreator() {};

	//to new a component based on the type set by ctor
	virtual Component* Create() = 0;

	//to store type of component to call create() 
	ComponentType typeId;

};

//used by addcomponent function, second argument, that specifies what type of game component to create
template<typename type>
class ComponentCreatorType : public ComponentCreator
{
public:
	ComponentCreatorType(ComponentType ID)
		:ComponentCreator(ID)
	{};
	virtual Component* Create()
	{
		return new type();
	}
};