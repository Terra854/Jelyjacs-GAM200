#include <Debug.h>
// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "Composition.h"

//don't think this is need for now
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
	//loop through components vector to find the type of game component
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
	//add into component vector
	Components.push_back(component);

	//sets the component's base ptr to refer to the current object that it is apart of
	component->Base = this;
}

void GameObjectComposition::Intialize()
{
	for (std::vector<GameComponent*>::iterator it = Components.begin(); it != Components.end(); ++it)
	{
		(*it)->Base = this;
		(*it)->Initialize();
	}
}