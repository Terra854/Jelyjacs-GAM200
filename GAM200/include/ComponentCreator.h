#pragma once
#include <Debug.h>

#include "component.h"
#include "ComponentType.h"

class ComponentCreator
{
public:
	ComponentCreator(ComponentTypeId);
	virtual ~ComponentCreator() {};

	virtual GameComponent* Create() = 0;

	ComponentTypeId typeId;

};

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