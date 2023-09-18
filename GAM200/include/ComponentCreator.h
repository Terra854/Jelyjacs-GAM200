#include "component.h"
#include "ComponentType.h"

class ComponentCreator
{
public:
	ComponentCreator(ComponentTypeId);
	virtual ~ComponentCreator();

	virtual GameComponent* Create() = 0;

	ComponentTypeId typeId;

};

template<typename type>
class ComponentCreatorType : public ComponentCreator
{
public:
	ComponentCreatorType(ComponentTypeId typeID) : ComponentCreator(typeID)
	{
		typeId = typeID;
	}
	virtual ~ComponentCreatorType();
	virtual GameComponent* Create()
	{
		return new type();
	}
};