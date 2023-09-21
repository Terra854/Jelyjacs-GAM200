#pragma once
#include <Debug.h>

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

/*
* This is the declaration of the game fatory class.
* It helps to create game objects in the game loop.
* Game objects are represented by GOC which is defined in Compositiion
* Game objects are containers for components which are themselves containers for data
* The code works off the data found in the data of game object components.
* Every game object is also given a unique ID number called GOCId which is assigned to them at creation time
* Every game object is also added to map and to access a specific game object in that map, you must have the object's ID
*/
#include "System.h"
#include "Interface_System.h"
#include "Composition.h"
#include "Mat3.h"
#include "Vec2.h"
#include <string>
#include <map>
#include <set>
#include <string>
#include "ComponentCreator.h"

class GameObjectFactory : public ISystems
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	GOC* createGameObj(const std::string& filename);

	void destroyGameObj(GOC* gameObject);

	virtual void Update(float time);

	virtual std::string SystemName() { return "Factory"; };

	//virtual void SendMessage(Message* msg);

	void destroyAllGameObjs();

	GOC* createEmptyGameObj();

	GOC* buildFromFile(const std::string& filename);

	void idGameObj(GOC* gameObj);

	GOC* getObjWithID(GOCId id);

	void AddComponentCreator(const std::string& name, ComponentCreator* creator);

	friend class Physics; // Needed to apply physics onto each object
private:
	unsigned lastGameObjID;


	//to map the component name in object text file to component type
	typedef std::map< std::string, ComponentCreator*> componentCreatorMap;
	componentCreatorMap componentMap;

	typedef std::map<unsigned, GOC*> gameObjIDMap;
	gameObjIDMap gameObjectMap;

	std::set<GOC*> gameObjsToBeDeleted;
};

extern GameObjectFactory* gameObjFactory;
