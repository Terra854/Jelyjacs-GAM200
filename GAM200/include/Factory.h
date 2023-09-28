#pragma once
#include <Debug.h>

/* !
@file    Factory.h
@author  w.jiahowjonathan@digipen.edu
@date   28/9/2023

This file contains the declaration of class Factory that helps to create gmae objects
according to object files. Game objects are represented as Object and the gmae objects
function as containers for components which are independent classes that give certain
the game objects different properties in the game world. Every game object has a unique
ID aand is stored as part of a private map
*//*__________________________________________________________________________*/

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

	size_t NumberOfObjects() {	return objectMap.size();}

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
