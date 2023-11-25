#pragma once
/* !
@file	Object.h
@author	Yeo Jia Ming
@date	28/9/2023

This file contains the declarations for the game object functions.
Contains member functions to add components into an object,
retrieve a specific component from the object,
and an id to defferntiate one object from another.
*//*__________________________________________________________________________*/
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

	void getKeysToArray(ComponentType keys[], int& size) const {
		size = 0;
		for (const auto& pair : Components) {
			keys[size++] = pair.first;
		}
	}
	///adds a component to be a part of the object by adding it into the component map
	void AddComponent(Component* component);

	void Initialize();

	///Get the game object's Id
	long GetId() { return ObjectId; }

	//Get the object's name
	std::string GetName() { return name; }

	int GetNumComponents() { return static_cast<int>(Components.size()); }

private:
	//contains all components of the current object
	std::unordered_map<ComponentType, Component*> Components;

	//A unique id for each object used to safely reference 
	//Objects.
	long ObjectId = -1;

	//A unique name for each object used to name objects
	std::string name;
};

