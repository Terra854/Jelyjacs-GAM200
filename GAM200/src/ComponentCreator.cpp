#include "ComponentCreator.h"
#include "ComponentType.h"

ComponentCreator::ComponentCreator(ComponentTypeId Id)
	:typeId(Id)
{}

template<typename type>
ComponentCreatorType<type>::ComponentCreatorType(ComponentTypeId Id)
	:ComponentCreator(Id)
{}

template<typename type>
GameComponent * ComponentCreatorType<type>::Create()
{
	return new type();
}