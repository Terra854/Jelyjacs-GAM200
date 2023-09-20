#pragma once

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties

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

		GameComponent* GetComponent(ComponentTypeId typeID);

		///Add an component used only for dynamic composition construction.
		void AddComponent(ComponentTypeId typeId, GameComponent* component);

		template<typename type>
		type* GetComponent_NoCast(ComponentTypeId typeId)
		{
			return static_cast<type*>(GetComponent(typeId));
		}

		///Get the game object's Id
		GOCId GetId() { return ObjectId; }


	private:
		std::vector<GameComponent*> Components;

		//A unique id for each object used to safely reference 
		//GOCs.
		GOCId ObjectId;

	};

