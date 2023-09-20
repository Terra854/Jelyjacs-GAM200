#pragma once
#include <Debug.h>

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "ComponentType.h"
#include "../../src/Assets Manager/serialization.h"

//Forward declaration of GOC class
class GameObjectComposition;
typedef GameObjectComposition GOC;

class GameComponent
{
public:
	friend class GameObjectComposition;

	virtual void Initialize() {};

	//GameComponent();
	virtual ~GameComponent(){};

	virtual void Serialize(Serialization& stream) {};

	///Get the GameObjectComposition this component is owned/composed.
	GOC* GetOwner() { return Base; }

	ComponentTypeId TypeId;

private:
	///Each component has a pointer back to the base owning composition.
	GOC* Base;
};


