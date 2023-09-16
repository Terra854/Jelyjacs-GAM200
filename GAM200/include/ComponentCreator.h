#include "ComponentType.h"

	//Forward declaration of component
	class GameComponent;

	///Game Component Creator Interface.
	///Used to create components in data driven method in the GameObjectFactory.
	class ComponentCreator
	{
	public:
		ComponentCreator(ComponentTypeId typeId)
			:TypeId(typeId)
		{
		}

		ComponentTypeId TypeId;
		///Create the component
		virtual GameComponent* Create() = 0;
		virtual ~ComponentCreator() {}
	};

	///Templated helper class used to make concrete component creators.
	template<typename type>
	class ComponentCreatorType : public ComponentCreator
	{
	public:
		ComponentCreatorType(ComponentTypeId typeId)
			:ComponentCreator(typeId)
		{
		}

		virtual GameComponent* Create()
		{
			return new type();
		}
	};