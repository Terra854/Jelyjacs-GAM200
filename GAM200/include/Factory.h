#pragma once
#include <Debug.h>

/*
* This is the declaration of the game fatory class.
* It helps to create game objects in the game loop.
* Game objects are represented by Object which is defined in Compositiion
* Game objects are containers for components which are themselves containers for data
* The code works off the data found in the data of game object components.
* Every game object is also given a unique ID number called long which is assigned to them at creation time
* Every game object is also added to map and to access a specific game object in that map, you must have the object's ID
*/
#include "Interface_System.h"
#include "Object.h"
#include "Mat3.h"
#include "Vec2.h"
#include <string>
#include <map>
#include <set>
#include <string>
#include "ComponentCreator.h"

class Factory : public ISystems
{
public:
	Factory();
	~Factory();

	Object* createObject(const std::string& filename);

	void destroyObject(Object* object);

	virtual void Update(float time);

	virtual std::string SystemName() { return "Factory"; };

	void destroyAllObjects();

	Object* createEmptyObject();

	//Object* buildFromFile(const std::string& filename);

	void assignIdToObject(Object* object);

	Object* getObjectWithID(long id);

	void AddComponentCreator(const std::string& name, BaseComponentCreator* creator);

	friend class PhysicsSystem; // Needed to apply physics onto each object
private:
	unsigned nextObjectId;

	//to map the component name in object text file to component type
	typedef std::map< std::string, BaseComponentCreator*> componentCreatorMap;
	componentCreatorMap componentMap;

	typedef std::map<unsigned, Object*> objectIDMap;
	objectIDMap objectMap;

	std::set<Object*> gameObjsToBeDeleted;
};

extern Factory* objectFactory;
