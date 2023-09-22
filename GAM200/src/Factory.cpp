#include <Debug.h>
#include "Factory.h"
#include "Assets Manager/text_serialization.h"
#include "Vec2.h"
#include "components/Transform.h"
#include <iostream>

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

/*
* Object is what the game object is represented by. It's definition is found in Object.h.
* As a reminder, a game object is just a group of components so the Object will have a vector of
* components
*/

Factory* objectFactory = NULL;

//Ctor
Factory::Factory()
{
	objectFactory = this;
	nextObjectId = 0;
}

//Dtor
Factory::~Factory()
{
	componentCreatorMap::iterator it = componentMap.begin();
	for  (; it != componentMap.end(); ++it)
	{
		delete it->second;
	}
}

//This creates a game object with a file name. This will be what is used to create game objects in the game loop
Object* Factory::createObject(const std::string& filename)
{
	Object* newGameObj = buildFromFile(filename);

	if (newGameObj)
	{
		newGameObj->Intialize();
	}

	return newGameObj;
}

//This doesn't destroy the game object instantly but will set it to be destroyed in the update loop
void Factory::destroyObject(Object* gameObject)
{
	gameObjsToBeDeleted.insert(gameObject);
}

//This deletes all objects to be deleted
void Factory::Update(float dt) {
	std::set<Object*>::iterator it = gameObjsToBeDeleted.begin();
	for (; it != gameObjsToBeDeleted.end(); ++it)
	{
		Object* gameObject = *it;
		objectIDMap::iterator idItr = objectMap.find(gameObject->ObjectId);
		//ErrorIf(idItr == gameObjectMap.end(), "Object %d was double deleted or is bad memory.", gameObject->ObjectId);
		if (idItr != objectMap.end())
		{
			//Delete it and remove its entry in the Id map
			delete gameObject;
			objectMap.erase(idItr);
		}
	}
	//All objects to be delete have been deleted
	gameObjsToBeDeleted.clear();
}
//This destroys all game objects
void Factory::destroyAllObjects()
{
	objectIDMap::iterator it = objectMap.begin();
	for (; it != objectMap.end(); ++it)
	{
		delete it->second;
	}

	objectMap.clear();
}

Object* Factory::createEmptyObject()
{
	Object* gameObj = new Object();
	assignIdToObject(gameObj);
	return gameObj;
}

//This creates an object from a text file. This needs to be tested a lot
Object* Factory::buildFromFile(const std::string& filename)
{
	//Open text file in read mode
	TextSerialization textStream;
	textStream.openFileRead(filename);

	// Make sure stream is valid
	if (textStream.isGood())
	{
		// Create new game object to hold components
		Object* gameObj = new Object();
		std::string componentName;

		while (textStream.isGood())
		{	
			// Read componentName
			textStream.readString(componentName);

			// Find the component Creator
			componentCreatorMap::iterator it = componentMap.find(componentName);
			if (it != componentMap.end())
			{	
				// Create Component by using the interface
				ComponentCreator* creator = it->second;
				Component* component = creator->Create();

				/* commented out to test the streamGet way of serialization
				* 
				GameComponent* testComponent = component;
				// Add component to composition
				gameObj->AddComponent(creator->typeId, component);
				if (componentName == "Transform")
				{
					Transform* trans = static_cast<Transform*>(testComponent);

					Vec2 pos;
					streamGet(textStream, pos.x);
					streamGet(textStream, pos.y);
					
					trans->Position = pos;

					float scale;
					streamGet(textStream, scale);
					trans->Scale = scale;

					float rotation;
					streamGet(textStream, rotation);
					trans->Rotation = rotation;
					
				} */

				streamGet(textStream, *component);
				gameObj->AddComponent(creator->typeId, component);

			}
		}
		// Id and initialize game object
		assignIdToObject(gameObj);

		return gameObj;
	}
	std::cout << "textstream not good" << std::endl;
	return NULL;
}

//This gives a game object an ID tag which can be used to find that same game object
void Factory::assignIdToObject(Object* gameObj)
{
	gameObj->ObjectId = nextObjectId;
	objectMap[nextObjectId] = gameObj;
	++nextObjectId;
}

Object* Factory::getObjectWithID(long id)
{
	objectIDMap::iterator it = objectMap.find(id);
	if (it != objectMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

//This adds a new component creator which is necessary for the creation of game objects
//Call this at the very start of the game loop in Intialize
void Factory::AddComponentCreator(const std::string& name, ComponentCreator* creator)
{
	componentMap[name] = creator;
}