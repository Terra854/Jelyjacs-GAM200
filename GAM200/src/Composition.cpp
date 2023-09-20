#include <Debug.h>
// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "Composition.h"
struct ComponentSorter {
	bool operator()(GameComponent* left, GameComponent* right) {
		return left->TypeId < right->TypeId;
	}
};

GameObjectComposition::GameObjectComposition()
{
	ObjectId = 0;
}

GameObjectComposition::~GameObjectComposition()
{

}

GameComponent* GameObjectComposition::GetComponent(ComponentTypeId typeID)
{
	for (GameComponent* c : Components) {
		if (c->TypeId == typeID)
			return c;
	}
	return nullptr;
}

void GameObjectComposition::AddComponent(ComponentTypeId typeId, GameComponent* component)
{
	//Store the components type Id
	component->TypeId = typeId;
	Components.push_back(component);

	component->Base = this;
}
