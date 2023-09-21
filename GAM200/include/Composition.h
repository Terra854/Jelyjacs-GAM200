#pragma once
#include <Debug.h>

// Need to change cause Ellie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "Component.h"
#include <vector>

	///Game Object Id Type
	typedef unsigned int GOCId;
	typedef GameObjectComposition GOC;

	class GameObjectComposition
	{
	public:

		GameObjectComposition();

		~GameObjectComposition();

		//Game object factory is the only class allowed to
		//create and destroy game objects.
		friend class GameObjectFactory;

		//returns a ptr to a specified type of game component of the object
		//if object does not contain that component, returns null
		GameComponent* GetComponent(ComponentTypeId typeID);

		///adds a component to be a part of the object by adding it into the components vector
		void AddComponent(ComponentTypeId typeId, GameComponent* component);


		///Get the game object's Id
		GOCId GetId() { return ObjectId; }


	private:
		//contains all components of the current object
		std::vector<GameComponent*> Components;

		//A unique id for each object used to safely reference 
		//GOCs.
		GOCId ObjectId;

	};

