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
#include "Assets Manager/json_serialization.h"
#include "Assets Manager/text_serialization.h"
#include "Vec2.h"
#include "components/Transform.h"
#include "components/Texture.h"
#include <iostream>
#include "components/Body.h"
#include "components/Physics.h"
#include "components/PlayerControllable.h"
#include "components/Animation.h"
#include "components/Event.h"
#include "Assets Manager/asset_manager.h"
#include "Object.h"

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
Object* Factory::createObject(const std::string& filename, bool addToObjectMap)
{
	// Check if the given file exists
	JsonSerialization jsonobj;
	jsonobj.openFileRead(filename);

	Object* obj = new Object();
	jsonobj.readString(obj->name, "Name");

	for (auto& component : jsonobj.read("Components")) {
		
		JsonSerialization jsonloop;
		jsonloop.jsonObject = &component;

		std::string type;
		jsonloop.readString(type, "Type");

		if (type == "Transform") {

			Transform* trans = (Transform*)((ComponentCreator<Transform>*) componentMap["Transform"])->Create();

			jsonloop.readFloat(trans->Position.x, "Properties", "Position", "x");
			jsonloop.readFloat(trans->Position.y, "Properties", "Position", "y");

			jsonloop.readFloat(trans->Scale.x, "Properties", "Scale_x");
			jsonloop.readFloat(trans->Scale.y, "Properties", "Scale_y");
			jsonloop.readFloat(trans->Rotation, "Properties", "Rotation");

			obj->AddComponent(trans);
		}
		else if (type == "Texture") {

			std::string path;
			jsonloop.readString(path, "Properties", "texturepath");
			bool exist = AssetManager::texturecheckexist(path);

			if (!exist) {
				std::cout << "Missing Texture!" << std::endl;
				AssetManager::addtextures(path);
			}
			else
			{
				GLuint texturepath = AssetManager::textureval(path);
			}
				Texture* tex = (Texture*)((ComponentCreator<Texture>*) componentMap["Texture"])->Create(path);
				obj->AddComponent(tex);
						
		}
		else if (type == "Body") {
			std::string shape;
			jsonloop.readString(shape, "Shape");
			if (shape == "Rectangle") {
				Rectangular* r = (Rectangular*)((ComponentCreator<Rectangular>*) componentMap["Rectangle"])->Create();

				if (jsonloop.isMember("Collision Response"))
					jsonloop.readBool(r->collision_response, "Collision Response");

				jsonloop.readFloat(r->width, "Properties", "width");
				jsonloop.readFloat(r->height, "Properties", "height");

				obj->AddComponent(r);
			}
			else if (shape == "Circle") {
				Circular* c = (Circular*)((ComponentCreator<Circular>*) componentMap["Circular"])->Create();

				if (jsonloop.isMember("Collision Response"))
					jsonloop.readBool(c->collision_response, "Collision Response");

				jsonloop.readFloat(c->circle.radius, "Properties", "radius");
				jsonloop.readFloat(c->circle.center.x, "Properties", "radius", "x");
				jsonloop.readFloat(c->circle.center.y, "Properties", "radius", "y");

				obj->AddComponent(c);
			}
			else if (shape == "Line") {
				Lines* l = (Lines*)((ComponentCreator<Lines>*) componentMap["Line"])->Create();
				
				if (jsonloop.isMember("Collision Response"))
					jsonloop.readBool(l->collision_response, "Collision Response");

				float x;
				float y;

				jsonloop.readFloat(x, "Properties", "start", "x");
				jsonloop.readFloat(y, "Properties", "start", "y");
				l->line.SetPt0({ x, y });

				jsonloop.readFloat(x, "Properties", "end", "x");
				jsonloop.readFloat(y, "Properties", "end", "y");
				l->line.SetPt1({ x, y });

				obj->AddComponent(l);
			}
		}
		else if (type == "Physics") {
			Physics* p = (Physics*)((ComponentCreator<Physics>*) componentMap["Physics"])->Create();

			float x;
			float y;

			jsonloop.readFloat(x, "Properties", "X_Velocity");
			jsonloop.readFloat(y, "Properties", "Y_Velocity");
			p->Velocity = Vec2(x, y);

			jsonloop.readFloat(p->Mass, "Properties", "Mass");

			jsonloop.readBool(p->AffectedByGravity, "Properties", "AffectedByGravity");

			obj->AddComponent(p);
		}
		else if (type == "Player") {
			PlayerControllable* p = (PlayerControllable*)((ComponentCreator<PlayerControllable>*) componentMap["Player"])->Create();
			obj->AddComponent(p);
		}
		else if (type == "Animation")
		{
			Animation* a = (Animation*)((ComponentCreator<Animation>*) componentMap["Animation"])->Create();

			std::string path;
			jsonloop.readString(path, "Properties", "texturepath");
			bool exist = AssetManager::animationcheckexist(path);
			if (!exist)
			{
				std::cout << "Missing Animation Texture!" << std::endl;
			}
			else
			{
				a->animation_tex_obj = AssetManager::animationval(path);
			}

			jsonloop.readFloat(a->frame_rate, "Properties", "framerate");

			if (jsonloop.isMember("jumpfixframe", "Properties"))
				jsonloop.readInt(a->jump_fixed_frame, "Properties", "jumpfixframe");

			float col;
			float row;
			jsonloop.readFloat(col, "Properties", "frame", 0);
			jsonloop.readFloat(row, "Properties", "frame", 1);

			bool faceright = true;

			// Create frame_map
			for (int j = 0; j < AnimationType::End; j++)
			{
				std::string animationtype;
				switch (j)
				{
					case AnimationType::Idle:
						animationtype = "idle";
						faceright = true;
						break;
					case AnimationType::Push:
						animationtype = "push";
						faceright = true;
						break;
					case AnimationType::Jump:
						animationtype = "jump";
						faceright = true;
						break;
					case AnimationType::Run:
						animationtype = "run";
						faceright = true;
						break;
					case AnimationType::Idle_left:
						animationtype = "idle";
						faceright = false;
						break;
					case AnimationType::Push_left:
						animationtype = "push";
						faceright = false;
						break;
					case AnimationType::Jump_left:
						animationtype = "jump";
						faceright = false;
						break;
					case AnimationType::Run_left:
						animationtype = "run";
						faceright = false;
						break;
					default:
						animationtype = "error";
						faceright = true;
						break;
				}

				AnimationType frametype = static_cast<AnimationType>(j);
				std::vector<GLApp::GLModel> animationmodel;

				// Ensure AnimationType exist before gathering data into animation_map
				
				if (jsonloop.isMember(animationtype, "Properties"))
				{
					float framecol;
					float framerow;
					jsonloop.readFloat(framecol, "Properties", animationtype, 0);
					jsonloop.readFloat(framerow, "Properties", animationtype, 1);

					for (int i = 0; i < framecol; i++)
					{
						GLApp::GLModel model = a->setup_texobj_animation(i/col, (framerow-1)/row, (i+1)/col, framerow/row ,faceright);
						animationmodel.push_back(model);
					}
				}

				a->animation_Map.emplace(frametype, animationmodel);
			}

			obj->AddComponent(a);
		}
		else if (type == "Event")
		{
			Event* e = (Event*)((ComponentCreator<Event>*) componentMap["Event"])->Create();

			jsonloop.readInt(e->linked_event, "Properties", "linkedevent");

			obj->AddComponent(e);
		}
	}

	// Run the initilization routines for each component (if there is any)
	obj->Intialize();

	// Add the new object to objectMap if needed
	if (addToObjectMap)
		assignIdToObject(obj);
	
	// Clean up
	jsonobj.closeFile();
	return obj;
}

//This doesn't destroy the game object instantly but will set it to be destroyed in the update loop
void Factory::destroyObject(Object* obj)
{
	gameObjsToBeDeleted.insert(obj);
}

//This deletes all objects to be deleted
void Factory::Update() {
	long temp_id;
	//bool delete_flag = false;
	std::set<Object*>::iterator it = gameObjsToBeDeleted.begin();
	for (; it != gameObjsToBeDeleted.end(); ++it)
	{
		Object* obj = *it;
		objectIDMap::iterator gameObjectInMap = objectMap.find(obj->ObjectId);
		if (gameObjectInMap != objectMap.end())
		{
			temp_id = obj->ObjectId;
			//Delete it and remove its entry in the Id map
			delete obj;
			objectMap.erase(gameObjectInMap);
			
			nextObjectId--;

			// Move all the next objects id down by one
			for (int i = temp_id; i < nextObjectId; i++) {
				objectMap[i] = objectMap[i + 1];
				objectMap[i + 1]->ObjectId--;
				objectMap.erase(i + 1);
			}
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
	nextObjectId = 0;
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

			trans->Scale = tran_pt->Scale;
			trans->Rotation = tran_pt->Rotation;

			obj->AddComponent(trans);
		}

		// Copy texture data
		else if (c.first == ComponentType::Texture)
		{
			Texture* tex = (Texture*)((ComponentCreator<Texture>*) componentMap["Texture"])->Create(std::string());
			Texture* tex_pt = static_cast<Texture*>(object->GetComponent(ComponentType::Texture));

			tex->textureName = tex_pt->textureName;
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
			p->Mass = p_pt->Mass;
			p->AffectedByGravity = p_pt->AffectedByGravity;

			obj->AddComponent(p);
		}
		// Clone player controllable data
		else if (c.first == ComponentType::PlayerControllable)
		{
			PlayerControllable* p = (PlayerControllable*)((ComponentCreator<PlayerControllable>*) componentMap["Player"])->Create();
			obj->AddComponent(p);
		}
		// Clone Animations data
		else if (c.first == ComponentType::Animation)
		{
			Animation* ani = (Animation*)((ComponentCreator<Animation>*) componentMap["Animation"])->Create();
			Animation* ani_tmp = static_cast<Animation*>(object->GetComponent(ComponentType::Animation));

			ani->animation_tex_obj = ani_tmp->animation_tex_obj;
			ani->animation_Map = ani_tmp->animation_Map;
			ani->frame_rate = ani_tmp->frame_rate;
			ani->jump_fixed_frame = ani_tmp->jump_fixed_frame;
			
			obj->AddComponent(ani);
		}
		else if (c.first == ComponentType::Event)
		{
			Event* e = (Event*)((ComponentCreator<Event>*) componentMap["Event"])->Create();
			Event* e_tmp = static_cast<Event*>(object->GetComponent(ComponentType::Event));


			e->linked_event = e_tmp->linked_event;

			obj->AddComponent(e);
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

Object* Factory::FindObject(std::string name)
{
	if (objectMap.empty())
	{
		return nullptr;
	}
	
	for (auto it = objectMap.begin(); it != objectMap.end(); it++)
	{
		Object* testObject = it->second;
		std::string objectName = testObject->GetName();
		if (objectName == name)
		{
			return testObject;
		}
	}

	return nullptr;
}

void Factory::DeleteComponent(unsigned id, ComponentType c) {
	Object* o = getObjectWithID(id);
	delete o->Components[c];
	o->Components.erase(c);
}