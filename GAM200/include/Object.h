#pragma once
#include <Debug.h>
#include "Component.h"
#include <vector>
#include <unordered_map>

class Object
{
public:

	Object();

	~Object();

	//Factory is the only class allowed to create and destroy objects.
	friend class Factory;

	//returns a ptr to a specified type of game component of the object
	//if object does not contain that component, returns null
	Component* GetComponent(ComponentType typeID);

	///adds a component to be a part of the object by adding it into the component map
	void AddComponent(Component* component);

	void Intialize();

	///Get the game object's Id
	long GetId() { return ObjectId; }

private:
	//contains all components of the current object
	std::unordered_map<ComponentType, Component*> Components;

	//A unique id for each object used to safely reference 
	//Objects.
	long ObjectId;

};

