#include <Debug.h>
// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "Object.h"

Object::Object()
{
	ObjectId = 0;
}

Object::~Object()
{

}

Component* Object::GetComponent(ComponentType typeID)
{
	//loop through components vector to find the type of game component
	for (Component* c : Components) {
		if (c->TypeId == typeID)
			return c;
	}
	return nullptr;
}

void Object::AddComponent(ComponentType typeId, Component* component)
{
	//Store the components type Id
	component->TypeId = typeId;
	//add into component vector
	Components.push_back(component);

	//sets the component's base ptr to refer to the current object that it is apart of
	component->Base = this;
}
