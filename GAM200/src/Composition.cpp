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
