
	//Forward declaration of GOC class
	class GameObjectComposition;
	typedef GameObjectComposition GOC;

	///A component is an atomic piece of functionality that is
	///composed onto a GameObjectComposition to form game objects.
	class GameComponent
	{
	public:
		friend class GameObjectComposition;

		///Signal that the component is now active in the game world.
		///See GameObjectComposition for details.
		virtual void Initialize() {};

		///Component Serialization Interface see Serialization.h for details.
		virtual void Serialize() { };

		///Get the GameObjectComposition this component is owned/composed.
		GOC* GetOwner() { return Base; }

		ComponentTypeId TypeId;

	protected:
		///Destroy the component.
		virtual ~GameComponent() {};
	private:
		///Each component has a pointer back to the base owning composition.
		GOC* Base;
	};


