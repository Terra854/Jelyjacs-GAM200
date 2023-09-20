#pragma once

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties

#include "ComponentType.h"

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

	///Get the GameObjectComposition this component is owned/composed.
	GOC* GetOwner() { return Base; }

	ComponentTypeId TypeId;

private:
	///Each component has a pointer back to the base owning composition.
	GOC* Base;
};


