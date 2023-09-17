#include "Composition.h"

	GameObjectComposition::GameObjectComposition()
	{

	}

	GameObjectComposition::~GameObjectComposition()
	{

	}

	void GameObjectComposition::AddComponent(ComponentTypeId typeId, GameComponent* component)
	{
		//Store the components type Id
		component->TypeId = typeId;
		Components.push_back(component);

		component->Base = this;
	}
