/* !
@file    Factory.h
@author  Jonathan Woo Jia How
@date   28/9/2023

This file contains the declaration of class Factory that helps to create gmae objects
according to object files. Game objects are represented as Object and the gmae objects
function as containers for components which are independent classes that give certain
the game objects different properties in the game world. Every game object has a unique
ID aand is stored as part of a private map
*//*__________________________________________________________________________*/

#pragma once
#include <Debug.h>
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
	//Ctor
	Factory();
	//Dtor
	~Factory();

	// This creates a game object using the variables from the json file
	Object* createObject(const std::string& filename);

	//This doesn't destroy the game object instantly but will set it to be destroyed in the update loop
	void destroyObject(Object* object);

	//This deletes all objects to be deleted
	virtual void Update();

	virtual std::string SystemName() { return "Factory"; };

	//This destroys all game objects
	void destroyAllObjects();

	//Creates a game object with no components
	Object* createEmptyObject();

	//This gives a game object an ID tag which can be used to find that same game object
	void assignIdToObject(Object* object);

	//Returns a game object from the map with a specific id
	Object* getObjectWithID(long id);

	//Returns the player object from the map
	Object* getPlayerObject();

	//This clones a game object
	Object* cloneObject(Object* object);

	//This adds a new component creator which is necessary for the creation of game objects
//Call this at the very start of the game loop in Intialize
	void AddComponentCreator(const std::string& name, BaseComponentCreator* creator);

	//Returns the total number of game objects created by this game factory
	size_t NumberOfObjects() {	return objectMap.size();}

	Object* FindObject(std::string name);

	void DeleteComponent(unsigned id, ComponentType c);

	int GetNextId() { return nextObjectId; };

	friend class PhysicsSystem; // Needed to apply physics onto each object
private:
	//ID to assign to the next game object created
	unsigned nextObjectId;

	//to map the component name in object text file to component type
	typedef std::map< std::string, BaseComponentCreator*> componentCreatorMap;
	componentCreatorMap componentMap;

	//Map of all the game objects created
	//Associated with an ID to return them when needed
	typedef std::map<unsigned, Object*> objectIDMap;
	objectIDMap objectMap;

	//Set of game objects to be deleted every frame
	std::set<Object*> gameObjsToBeDeleted;
};

extern Factory* objectFactory;
