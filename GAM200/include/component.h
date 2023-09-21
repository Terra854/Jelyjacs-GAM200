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

	//to initialise remainng game components not set from text file
	virtual void Initialize() {};

	GameComponent() = default;
	virtual ~GameComponent() = default;

	//for serialization to know how far to read in text file to get all the variables of this game component
	virtual void Serialize(Serialization& stream) {};

	///to get the object that this component is part of
	GOC* GetOwner() { return Base; }

	//to differentiate between the components
	ComponentTypeId TypeId;

private:
	///to refer to the object that component is a part of
	GOC* Base;
};


