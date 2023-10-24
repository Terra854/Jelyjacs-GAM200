/* !
@file    Factory.cpp
@author  Jonathan Woo Jia How
@date   28/9/2023

This file contains the definition of class Factory that helps to create gmae objects
according to object files. Game objects are represented as Object and the gmae objects
function as containers for components which are independent classes that give certain
the game objects different properties in the game world. Every game object has a unique
ID aand is stored as part of a private map
*//*__________________________________________________________________________*/

#include <json/json.h> // External libraries - put those headers before Debug.h

#include <Debug.h>
#include "Factory.h"
#include "Assets Manager/text_serialization.h"
#include "Vec2.h"
#include "components/Transform.h"
#include "components/Texture.h"
#include <iostream>
#include "components/Body.h"
#include "components/Physics.h"
#include "components/PlayerControllable.h"
#include "Assets Manager/asset_manager.h"

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
	// Delete all component creators
	for (componentCreatorMap::iterator it = componentMap.begin(); it != componentMap.end(); ++it)
	{
		delete it->second;
	}
	componentMap.clear();

	// Delete all objects
	destroyAllObjects();
}

// This creates a game object using the variables from the json file
// This will be what is used to create game objects in the game loop
Object* Factory::createObject(const std::string& filename)
{
	// Check if the given file exists
	std::ifstream jsonFile(filename);
	if (!jsonFile.is_open()) {
		std::cerr << "Could not open file: " << filename << std::endl;
		return NULL;
	}

	Json::Value* jsonObject = new Json::Value;;
	Json::Reader reader;

	// Check if the given file is a valid json file
	if (!reader.parse(jsonFile, *jsonObject)) {
		std::cout << "Failed to parse JSON" << std::endl;
		jsonFile.close();
		return NULL;
	}
	jsonFile.close();

	// Now parse the file to populate the object with components

	Object* obj = new Object();
	obj->name = (*jsonObject)["Name"].asString();

	for (const auto& component : (*jsonObject)["Components"]) {
		std::string type = component["Type"].asString();

		if (type == "Transform") {

			Transform* trans = (Transform*)((ComponentCreator<Transform>*) componentMap["Transform"])->Create();

			trans->Position.x = component["Properties"]["Position"]["x"].asFloat();
			trans->Position.y = component["Properties"]["Position"]["y"].asFloat();

			//trans->PrevPosition = trans->Position;

			trans->Scale_x = component["Properties"]["Scale_x"].asFloat();
			trans->Scale_y = component["Properties"]["Scale_y"].asFloat();
			trans->Rotation = component["Properties"]["Rotation"].asFloat();

			trans->Matrix = Mat3Scale(trans->Scale_x, trans->Scale_y) * Mat3RotDeg(trans->Rotation) * Mat3Translate(trans->Position.x, trans->Position.y);

			obj->AddComponent(trans);
		}
		else if (type == "Texture") {

			bool exist = AssetManager::texturecheckexist(component["Properties"]["texturepath"].asString());

			if (!exist)
				std::cout << "Missing Texture!" << std::endl;
			else
			{
				GLuint texturepath = AssetManager::textureval(component["Properties"]["texturepath"].asString());
				Texture* tex = (Texture*)((ComponentCreator<Texture>*) componentMap["Texture"])->Create(texturepath);
				obj->AddComponent(tex);
			}			
		}
		else if (type == "Body") {
			std::string shape = component["Shape"].asString();
			if (shape == "Rectangle") {
				Rectangular* r = (Rectangular*)((ComponentCreator<Rectangular>*) componentMap["Rectangle"])->Create();

				r->width = component["Properties"]["width"].asFloat();
				r->height = component["Properties"]["height"].asFloat();

				obj->AddComponent(r);
			}
			else if (shape == "Circle") {
				Circular* c = (Circular*)((ComponentCreator<Circular>*) componentMap["Circular"])->Create();

				c->circle.radius = component["Properties"]["radius"].asFloat();
				c->circle.center.x = component["Properties"]["center"]["x"].asFloat();
				c->circle.center.y = component["Properties"]["center"]["y"].asFloat();

				obj->AddComponent(c);
			}
			else if (shape == "Line") {
				Lines* l = (Lines*)((ComponentCreator<Lines>*) componentMap["Line"])->Create();

				l->line.SetPt0({ component["Properties"]["start"]["x"].asFloat(), component["Properties"]["start"]["y"].asFloat() });
				l->line.SetPt1({ component["Properties"]["end"]["x"].asFloat(), component["Properties"]["end"]["y"].asFloat() });

				obj->AddComponent(l);
			}
		}
		else if (type == "Physics") {
			Physics* p = (Physics*)((ComponentCreator<Physics>*) componentMap["Physics"])->Create();

			p->Velocity = Vec2(component["Properties"]["X_Velocity"].asFloat(), component["Properties"]["Y_Velocity"].asFloat());
			p->Y_Acceleration = component["Properties"]["Y_Acceleration"].asFloat();
			p->Mass = component["Properties"]["Mass"].asFloat();

			obj->AddComponent(p);
		}
		else if (type == "Player") {
			PlayerControllable* p = (PlayerControllable*)((ComponentCreator<PlayerControllable>*) componentMap["Player"])->Create();
			obj->AddComponent(p);
		}
	}

	// Run the initilization routines for each component (if there is any)
	obj->Intialize();

	// Add the new object to objectMap and clean up
	assignIdToObject(obj);
	delete jsonObject;
	return obj;
}

//This doesn't destroy the game object instantly but will set it to be destroyed in the update loop
void Factory::destroyObject(Object* obj)
{
	gameObjsToBeDeleted.insert(obj);
}

//This deletes all objects to be deleted
void Factory::Update() {
	std::set<Object*>::iterator it = gameObjsToBeDeleted.begin();
	for (; it != gameObjsToBeDeleted.end(); ++it)
	{
		Object* obj = *it;
		objectIDMap::iterator gameObjectInMap = objectMap.find(obj->ObjectId);
		if (gameObjectInMap != objectMap.end())
		{
			//Delete it and remove its entry in the Id map
			delete obj;
			objectMap.erase(gameObjectInMap);
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

//Creates a game object with no components
Object* Factory::createEmptyObject()
{
	Object* obj = new Object();
	assignIdToObject(obj);
	return obj;
}

//This gives a game object an ID tag which can be used to find that same game object
void Factory::assignIdToObject(Object* gameObj)
{
	gameObj->ObjectId = nextObjectId;
	objectMap[nextObjectId] = gameObj;
	++nextObjectId;
}

//Returns a game object from the map with a specific id
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

// Returns the player object from the map
Object* Factory::getPlayerObject()
{
	for (auto& obj : objectMap) {
		if (obj.second->GetComponent(ComponentType::PlayerControllable))
			return obj.second; // There can only be 1 player object at this time
	}
	return nullptr;
}

//This clones an object
Object* Factory::cloneObject(Object* object)
{
	Object* obj = createEmptyObject();

	// Clone the object name
	obj->name = object->GetName();

	// Clone all components (specific copying might be better to be at their component itself)
	for (const std::pair<ComponentType, Component*>& c : object->Components)
	{
		// Copy transform data
		if (c.first == ComponentType::Transform) 
		{
			Transform* trans = (Transform*)((ComponentCreator<Transform>*) componentMap["Transform"])->Create();
			Transform* tran_pt = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));

			trans->Position.x = tran_pt->Position.x;
			trans->Position.y = tran_pt->Position.y;

			trans->PrevPosition = tran_pt->PrevPosition;

			trans->Scale_x = tran_pt->Scale_x;
			trans->Scale_y = tran_pt->Scale_y;
			trans->Rotation = tran_pt->Rotation;

			trans->Matrix = tran_pt->Matrix;

			// Forced move cloned object by y + 100 for testing
			trans->Position.x -= 100;
			trans->Position.y += 100;

			obj->AddComponent(trans);
		}

		// Copy texture data
		else if (c.first == ComponentType::Texture)
		{
			Texture* tex = (Texture*)((ComponentCreator<Texture>*) componentMap["Texture"])->Create(NULL);
			Texture* tex_pt = static_cast<Texture*>(object->GetComponent(ComponentType::Texture));

			tex->texturepath = tex_pt->texturepath;
			obj->AddComponent(tex);
		}
		// Clone body data
		else if (c.first == ComponentType::Body)
		{
			Body* body_pt = static_cast<Body*>(object->GetComponent(ComponentType::Body));
			
			if (body_pt->GetShape() == Shape::Rectangle) {
				Rectangular* r = (Rectangular*)((ComponentCreator<Rectangular>*) componentMap["Rectangle"])->Create();

				r->width = ((Rectangular*)body_pt)->width;
				r->height = ((Rectangular*)body_pt)->height;
				r->aabb = ((Rectangular*)body_pt)->aabb;
				r->collision_flag = ((Rectangular*)body_pt)->collision_flag;

				obj->AddComponent(r);
			}
			else if (body_pt->GetShape() == Shape::Circle) {
				Circular* c = (Circular*)((ComponentCreator<Circular>*) componentMap["Circular"])->Create();

				c->circle = ((Circular*)body_pt)->circle;

				obj->AddComponent(c);
			}
			else if (body_pt->GetShape() == Shape::Line) {
				Lines* l = (Lines*)((ComponentCreator<Lines>*) componentMap["Line"])->Create();

				l->line = ((Lines*)body_pt)->line;

				obj->AddComponent(l);
			}
		}
		// Clone physics data
		else if (c.first == ComponentType::Physics)
		{
			Physics* p = (Physics*)((ComponentCreator<Physics>*) componentMap["Physics"])->Create();
			Physics* p_pt = static_cast<Physics*>(object->GetComponent(ComponentType::Physics));

			p->Velocity = p_pt->Velocity;
			p->Y_Acceleration = p_pt->Y_Acceleration;
			p->Mass = p_pt->Mass;

			obj->AddComponent(p);
		}
		// Clone player controllable data
		if (c.first == ComponentType::PlayerControllable)
		{
			PlayerControllable* p = (PlayerControllable*)((ComponentCreator<PlayerControllable>*) componentMap["Player"])->Create();
			obj->AddComponent(p);
		}
	}

	obj->Intialize();
	return obj;
}

//This adds a new component creator which is necessary for the creation of game objects
//Call this at the very start of the game loop in Intialize
void Factory::AddComponentCreator(const std::string& name, BaseComponentCreator* creator)
{
	componentMap[name] = creator;
}