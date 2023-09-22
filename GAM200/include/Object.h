#pragma once
#include <Debug.h>

// Need to change cause Ellie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "Component.h"
#include <vector>

	class Object
	{
	public:

		Object();

		~Object();

		//Game object factory is the only class allowed to
		//create and destroy game objects.
		friend class Factory;

		//returns a ptr to a specified type of game component of the object
		//if object does not contain that component, returns null
		Component* GetComponent(ComponentType typeID);

		///adds a component to be a part of the object by adding it into the components vector
		void AddComponent(ComponentType typeId, Component* component);

		void Intialize();

		///Get the game object's Id
		long GetId() { return ObjectId; }

	private:
		//contains all components of the current object
		std::vector<Component*> Components;

		//A unique id for each object used to safely reference 
		//Objects.
		long ObjectId;

	};

