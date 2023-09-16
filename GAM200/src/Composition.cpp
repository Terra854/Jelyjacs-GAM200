#include "Composition.h"

	//Used to sort components using their type Id.
	struct ComponentSorter
	{
		bool operator()(GameComponent* left, GameComponent* right)const
		{
			return left->TypeId < right->TypeId;
		}
	};

	void GameObjectComposition::Initialize()
	{
		//Initialize every component on this composition
		//and set their composition owner. This allows each
		//component to initialize itself separate from its constructor.
		//Which is need for serialization and to allow components to get
		//pointers to each other during initialization.
		for (ComponentIt it = Components.begin(); it != Components.end(); ++it)
		{
			(*it)->Base = this;
			(*it)->Initialize();
		}
	}

	GameObjectComposition::GameObjectComposition()
	{
		ObjectId = 0;
	}

	GameObjectComposition::~GameObjectComposition()
	{
		//Delete each component using the component's virtual destructor
		//takes care of all resources and memory.
		for (ComponentIt it = Components.begin(); it != Components.end(); ++it)
			delete* it;
	}

	void GameObjectComposition::AddComponent(ComponentTypeId typeId, GameComponent* component)
	{
		//Store the components type Id
		component->TypeId = typeId;
		Components.push_back(component);
	}

	void GameObjectComposition::Destroy()
	{
		//Signal the factory that is object needs to be destroyed
		//this will happen at the end of the frame.
		FACTORY->Destroy(this);
	}
