
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

		///Add an component used only for dynamic composition construction.
		void AddComponent(ComponentTypeId typeId, GameComponent* component);

		///Get the game object's Id
		GOCId GetId() { return ObjectId; }


	private:
		std::vector<GameComponent*> Components;

		//A unique id for each object used to safely reference 
		//GOCs.
		GOCId ObjectId;

	};

