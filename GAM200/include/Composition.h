
#include "Component.h"
#include <vector>
#include "ComponentType.h"

	///Game Object Id Type
	typedef unsigned int GOCId;


	//A vector of components pointers is used to store components.
	typedef std::vector<GameComponent*> ComponentArray;

	class GameObjectComposition
	{
	public:

		//Game object factory is the only class allowed to
		//create and destroy game objects.
		friend class GameObjectFactory;


		///Type safe way of accessing components.
		template<typename type>
		type* GetComponentType(ComponentTypeId typeId);

		///Initialize all the components on the composition.
		///See function for details.
		void Initialize();

		///Correctly destroys an object by adding it to the factories destroy list.
		///Object will be destroy the next time the factory updates.
		void Destroy();

		///Add an component used only for dynamic composition construction.
		void AddComponent(ComponentTypeId typeId, GameComponent* component);

		///Get the game object's Id
		GOCId GetId() { return ObjectId; }
	private:

		//Sorted array of components.
		ComponentArray Components;
		typedef ComponentArray::iterator ComponentIt;

		//A unique id for each object used to safely reference 
		//GOCs.
		GOCId ObjectId;

		//The constructor and destructor are private to prevent a user
		//from creating or deleting an object directly they must use the Destroy function

		//FACTORY ONLY: Use CreateEmptyComposition on factory instead
		GameObjectComposition();
		//FACTORY ONLY: Use Destroy instead, factory will call the destructor correctly
		~GameObjectComposition();

	};

	//A Short name for GameObjectComposition
	typedef GameObjectComposition GOC;

	//A more advanced type safe way of accessing components.
	//Interface becomes Transform* transform = object->has(Transform);
	template<typename type>
	type* GameObjectComposition::GetComponentType(ComponentTypeId typeId)
	{
		return static_cast<type*>(GetComponent(typeId));
	}

