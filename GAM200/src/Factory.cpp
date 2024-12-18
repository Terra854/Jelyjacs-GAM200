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
#include "components/Text.h"
#include "Assets Manager/asset_manager.h"
#include "Object.h"
#include <components/Behaviour.h>
#include "GameLogic.h"
#include <SceneManager.h>
#include <LevelEditor.h>
#include <components/Dialogue.h>

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

	// In order to use the game object factory, we need to register the components we want to use first like this
	// When we create new types of components, we need to add it in there as well
	AddComponentCreator("Transform", new ComponentCreator<Transform>());
	AddComponentCreator("Texture", new ComponentCreator<Texture>());
	AddComponentCreator("Rectangle", new ComponentCreator<Rectangular>());
	AddComponentCreator("Circle", new ComponentCreator<Circular>());
	AddComponentCreator("Line", new ComponentCreator<Lines>());
	AddComponentCreator("Physics", new ComponentCreator<Physics>());
	AddComponentCreator("Player", new ComponentCreator<PlayerControllable>());
	AddComponentCreator("Event", new ComponentCreator<Event>());
	AddComponentCreator("Behaviour", new ComponentCreator<Behaviour>());
	AddComponentCreator("Animation", new ComponentCreator<Animation>());
	AddComponentCreator("Text", new ComponentCreator<Text>());
	AddComponentCreator("Dialogue", new ComponentCreator<Dialogue>());
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
Object* Factory::createObject(const std::string& filename)
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

			jsonloop.readFloat(trans->Scale.x, "Properties", "Scale", "x");
			jsonloop.readFloat(trans->Scale.y, "Properties", "Scale", "y");
			jsonloop.readFloat(trans->Rotation, "Properties", "Rotation");

			obj->AddComponent(trans);
		}
		else if (type == "Texture") {

			std::string path;
			jsonloop.readString(path, "Properties", "texturepath");
			bool exist = AssetManager::texturecheckexist(path);

			if (!exist) {
				std::cout << "Missing Texture!" << std::endl;
				// Attempt to add the texture
				AssetManager::addtextures(path);
			}

			Texture* tex = (Texture*)((ComponentCreator<Texture>*) componentMap["Texture"])->Create(path);

			jsonloop.readFloat(tex->opacity, "Properties", "opacity");
			obj->AddComponent(tex);

		}
		else if (type == "Body") {
			std::string shape;
			jsonloop.readString(shape, "Shape");
			if (shape == "Rectangle") {
				Rectangular* r = (Rectangular*)((ComponentCreator<Rectangular>*) componentMap["Rectangle"])->Create();

				if (jsonloop.isMember("Collision Response"))
					jsonloop.readBool(r->collision_response, "Collision Response");

				if (jsonloop.isMember("Active"))
					jsonloop.readBool(r->active, "Active");

				jsonloop.readFloat(r->width, "Properties", "width");
				jsonloop.readFloat(r->height, "Properties", "height");
				jsonloop.readBool(r->pushable, "Properties", "pushable");

				int material = 0;
				jsonloop.readInt(material, "Properties", "material");
				r->material = static_cast<Material>(material);

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

			jsonloop.readBool(p->AbleToPushObjects, "Properties", "AbleToPushObjects");

			obj->AddComponent(p);
		}
		else if (type == "Player") {
			PlayerControllable* p = (PlayerControllable*)((ComponentCreator<PlayerControllable>*) componentMap["Player"])->Create();
			obj->AddComponent(p);
		}
		else if (type == "Animation") // @guochen
		{
			Animation* a = (Animation*)((ComponentCreator<Animation>*) componentMap["Animation"])->Create();

			std::string path;

			jsonloop.readString(path, "Properties", "texturepath");
			bool exist = AssetManager::animationcheckexist(path);
			if (!exist)
			{
				std::cout << "Missing Animation Texture!" << std::endl;
				continue; // No animations, skipped adding component
			}
			else
			{
				a->animation_tex_obj = AssetManager::animationval(path);
			}

			jsonloop.readFloat(a->frame_rate, "Properties", "framerate");

			if (jsonloop.isMember("jumpfixframe", "Properties"))
				jsonloop.readInt(a->jump_fixed_frame, "Properties", "jumpfixframe");

			if (jsonloop.isMember("opacity", "Properties"))
				jsonloop.readFloat(a->opacity, "Properties", "opacity");

			if (jsonloop.isMember("faceright", "Properties"))
				jsonloop.readBool(a->face_right, "Properties", "faceright");

			jsonloop.readFloat(a->animation_scale.first, "Properties", "frame", 0);
			jsonloop.readFloat(a->animation_scale.second, "Properties", "frame", 1);

			// Create frame_map
			for (int j = 1; j <= a->animation_scale.second; j++)
			{
				std::pair<int, AnimationType> animationframesecond;

				std::vector<GLApp::GLModel> animationmodel;

				// Ensure AnimationType exist before gathering data into animation_map

				if (jsonloop.isMember(std::to_string(j), "Properties"))
				{
					int framecol;
					std::string animationtype;

					jsonloop.readInt(framecol, "Properties", std::to_string(j), 0);
					jsonloop.readString(animationtype, "Properties", std::to_string(j), 1);

					AnimationType anitype = stringToAnimationType(animationtype);

					animationframesecond.first = framecol;
					animationframesecond.second = anitype;

					a->animation_frame.emplace(j, animationframesecond);

					// Create left facing version of the animations if necessary
					if (a->face_right == false)
					{
						switch (anitype)
						{
						case AnimationType::Idle:
							animationframesecond.second = AnimationType::Idle_left;
							break;
						case AnimationType::Push:
							animationframesecond.second = AnimationType::Push_left;
							break;
						case AnimationType::Jump:
							animationframesecond.second = AnimationType::Jump_left;
							break;
						case AnimationType::Run:
							animationframesecond.second = AnimationType::Run_left;
							break;
						case AnimationType::Teleport:
							animationframesecond.second = AnimationType::Teleport_left;
							break;
						default:
							animationframesecond.second = AnimationType::No_Animation_Type;
							break;
						}

						//std::cout << "Creating left variant\n";

						// j needs to be different from non left version (j+scale)
						a->animation_frame.emplace(j + static_cast<int>(a->animation_scale.second), animationframesecond);
					}

					// Special case, number of rows indicated is more than initial declared but still exist in json data
					// Normally only used in cases where there is only a row but you want different frames for the same row
					int g = j + 1;
					while (g > a->animation_scale.second && jsonloop.isMember(std::to_string(g), "Properties"))
					{
						jsonloop.readInt(framecol, "Properties", std::to_string(g), 0);
						jsonloop.readString(animationtype, "Properties", std::to_string(g), 1);

						anitype = stringToAnimationType(animationtype);

						animationframesecond.first = framecol;
						animationframesecond.second = anitype;

						// Still creating animation frame with different framecol, will properly calculate in set_up_map
						a->animation_frame.emplace(g, animationframesecond);
						g++;
					} 
				}
			}

			a->set_up_map(false);

			// Set it back to facing right by default after the creations of animation map
			a->face_right = true;
			obj->AddComponent(a);
		}
		else if (type == "Event")
		{
			Event* e = (Event*)((ComponentCreator<Event>*) componentMap["Event"])->Create();

			jsonloop.readInt(e->linked_event, "Properties", "linkedevent");

			obj->AddComponent(e);
		}
		else if (type == "Behaviour")
		{
			Behaviour* b = (Behaviour*)((ComponentCreator<Behaviour>*) componentMap["Behaviour"])->Create();
			std::string temp_name;
			int temp_index;
			jsonloop.readString(temp_name, "Properties", "Script");
			jsonloop.readInt(temp_index, "Properties", "Index");
			obj->AddComponent(b);
			Logic->AddBehaviourComponent(b);
			std::cout << "Behaviour Script & Index: " << temp_name << ", " << temp_index << std::endl;
			b->SetBehaviourIndex(temp_index);
			b->SetBehaviourName(temp_name);
		}
		else if (type == "Text")
		{
			Text* t = (Text*)((ComponentCreator<Text>*) componentMap["Text"])->Create();
			jsonloop.readString(t->text, "Properties", "text");
			
			if (jsonloop.isMember("fontSize", "Properties"))
				jsonloop.readFloat(t->fontSize, "Properties", "fontSize");

			obj->AddComponent(t);
		}

		else if (type == "Dialogue")
		{
			Dialogue* d = (Dialogue*)((ComponentCreator<Dialogue>*) componentMap["Dialogue"])->Create();
			std::string dialogue;
			jsonloop.readString(dialogue, "Properties", "dialogue");
			d->ChangeDialogue(dialogue);
			obj->AddComponent(d);
		}
	}

	// Run the initialization routines for each component (if there is any)
	obj->Initialize();

	// Clean up
	jsonobj.closeFile();
	return obj;
}

void Factory::saveObject(std::string filename, Object* obj) {

	Json::Value jsonobj;

	Transform* tr = (Transform*)obj->GetComponent(ComponentType::Transform);
	Texture* te = (Texture*)obj->GetComponent(ComponentType::Texture);
	Body* bo = (Body*)obj->GetComponent(ComponentType::Body);
	Physics* ph = (Physics*)obj->GetComponent(ComponentType::Physics);
	PlayerControllable* pc = (PlayerControllable*)obj->GetComponent(ComponentType::PlayerControllable);
	Animation* a = (Animation*)obj->GetComponent(ComponentType::Animation);
	Event* e = (Event*)obj->GetComponent(ComponentType::Event);
	Behaviour* be = (Behaviour*)obj->GetComponent(ComponentType::Behaviour);
	Text* t = (Text*)obj->GetComponent(ComponentType::Text);
	Dialogue* d = (Dialogue*)obj->GetComponent(ComponentType::Dialogue);

	jsonobj["Name"] = obj->name;

	// Transform
	if (tr != nullptr) {
		Json::Value transform;
		transform["Type"] = "Transform";
		transform["Properties"]["Position"]["x"] = tr->Position.x;
		transform["Properties"]["Position"]["y"] = tr->Position.y;
		transform["Properties"]["Scale"]["x"] = tr->Scale.x;
		transform["Properties"]["Scale"]["y"] = tr->Scale.y;
		transform["Properties"]["Rotation"] = tr->Rotation;
		jsonobj["Components"].append(transform);
	}

	// Texture
	if (te != nullptr) {
		Json::Value texture;
		texture["Type"] = "Texture";
		texture["Properties"]["texturepath"] = te->textureName;
		texture["Properties"]["opacity"] = te->opacity;
		jsonobj["Components"].append(texture);

	}

	// Animation 
	if (a != nullptr) {
		Json::Value animation;
		animation["Type"] = "Animation";

		for (const auto& pair : AssetManager::animations) {
			if (pair.second == a->animation_tex_obj) {
				animation["Properties"]["texturepath"] = pair.first;
				break; // Stop searching after the first match is found
			}
		}

		animation["Properties"]["jumpfixframe"] = a->jump_fixed_frame;
		animation["Properties"]["framerate"] = a->frame_rate;
		animation["Properties"]["opacity"] = a->opacity;
		animation["Properties"]["faceright"] = a->face_right;
		animation["Properties"]["frame"][0] = a->animation_scale.first;
		animation["Properties"]["frame"][1] = a->animation_scale.second;

		for (auto& f : a->animation_frame) {
			Json::Value frame;
			frame[0] = f.second.first;
			std::string type = AnimationTypeToString(f.second.second);
			std::transform(type.begin(), type.end(), type.begin(),
				[](unsigned char c) { return static_cast<char>(std::tolower(static_cast<int>(c))); });
			frame[1] = type;
			animation["Properties"][std::to_string(f.first)] = frame;
		}

		jsonobj["Components"].append(animation);

	}

	// Text
	if (t != nullptr) {
		Json::Value text;
		text["Type"] = "Text";
		text["Properties"]["text"] = t->text;
		text["Properties"]["fontSize"] = t->fontSize;
		jsonobj["Components"].append(text);
	}


	// Body
	if (bo != nullptr) {
		Json::Value body;
		body["Type"] = "Body";
		if (bo->GetShape() == Shape::Rectangle) {
			Rectangular* r = (Rectangular*)bo;
			body["Shape"] = "Rectangle";
			body["Properties"]["width"] = r->width;
			body["Properties"]["height"] = r->height;
			body["Properties"]["pushable"] = r->pushable;
			body["Properties"]["material"] = static_cast<int>(r->material);
			body["Properties"]["active"] = r->active;
			body["Properties"]["collision_response"] = r->collision_response;
		}
		else if (bo->GetShape() == Shape::Circle) {
			Circular* c = (Circular*)bo;
			body["Shape"] = "Circle";
			body["Properties"]["radius"] = c->circle.radius;
			body["Properties"]["center"]["x"] = c->circle.center.x;
			body["Properties"]["center"]["y"] = c->circle.center.y;
			body["Properties"]["collision_response"] = c->collision_response;
		}
		else if (bo->GetShape() == Shape::Line) {
			Lines* l = (Lines*)bo;
			body["Shape"] = "Line";
			body["Properties"]["start"]["x"] = l->line.Pt0().x;
			body["Properties"]["start"]["y"] = l->line.Pt0().y;
			body["Properties"]["end"]["x"] = l->line.Pt1().x;
			body["Properties"]["end"]["y"] = l->line.Pt1().y;
			body["Properties"]["collision_response"] = l->collision_response;
		}
		jsonobj["Components"].append(body);

	}

	// Physics
	if (ph != nullptr) {
		Json::Value physics;
		physics["Type"] = "Physics";
		physics["Properties"]["X_Velocity"] = ph->Velocity.x;
		physics["Properties"]["Y_Velocity"] = ph->Velocity.y;
		physics["Properties"]["Mass"] = ph->Mass;
		physics["Properties"]["AffectedByGravity"] = ph->AffectedByGravity;
		physics["Properties"]["AbleToPushObjects"] = ph->AbleToPushObjects;
		jsonobj["Components"].append(physics);
	}

	// PlayerControllable
	if (pc != nullptr) {
		Json::Value player;
		player["Type"] = "Player";
		jsonobj["Components"].append(player);
	}

	// Event
	if (e != nullptr) {
		Json::Value event;
		event["Type"] = "Event";
		event["Properties"]["linkedevent"] = e->linked_event;
		jsonobj["Components"].append(event);
	}

	// Behaviour
	if (be != nullptr) {
		Json::Value behaviour;
		behaviour["Type"] = "Behaviour";
		behaviour["Properties"]["Script"] = be->GetBehaviourName();
		behaviour["Properties"]["Index"] = be->GetBehaviourIndex();
		jsonobj["Components"].append(behaviour);
	}
	
	//dialogue
	if (d != nullptr) {
		Json::Value dialogue;
		dialogue["Type"] = "Dialogue";
		dialogue["Properties"]["dialogue"] = d->GetEntireDialogue();
		jsonobj["Components"].append(dialogue);
	}
	

	// Save the object to a file

	std::ofstream outputFile("Asset/Objects/" + filename);
	if (outputFile.is_open()) {
		Json::StreamWriterBuilder writer;
		writer["indentation"] = "  ";

		outputFile << Json::writeString(writer, jsonobj);
		outputFile.close();
		std::cout << filename << " has been successfully saved." << std::endl;
		return;
	}
	else
		std::cerr << "Failed to open file for writing." << std::endl;
}

//This doesn't destroy the game object instantly but will set it to be destroyed in the update loop
void Factory::destroyObject(Object* obj)
{
	gameObjsToBeDeleted.insert(obj);
}

//This deletes all objects to be deleted
void Factory::Update() {
	long temp_id;
	std::set<Object*>::iterator it = gameObjsToBeDeleted.begin();
	for (; it != gameObjsToBeDeleted.end(); ++it)
	{
		Object* obj = *it;
		objectIDMap::iterator gameObjectInMap = objectMap.find(obj->ObjectId);
		if (gameObjectInMap != objectMap.end())
		{
			temp_id = obj->ObjectId;

			// Delete the reference to the object in the layer
			for (auto& l : SceneManager::layers) {
				std::vector<Object*>& v = l.second.second;
				auto it2 = std::find(v.begin(), v.end(), obj);
				if (it2 != v.end()) {
					v.erase(it2);
					break; // The reference is deleted. Stop the loop
				}
			}

			//Delete it and remove its entry in the Id map
			delete obj;
			objectMap.erase(gameObjectInMap);

			nextObjectId--;

			// Move all the next objects id down by one
			for (int i = temp_id; i < nextObjectId; i++) {
				objectMap[i] = objectMap[i + 1];
				objectMap[i + 1]->ObjectId--;
				objectMap.erase((size_t)(i + 1));
			}

#if defined(DEBUG) | defined(_DEBUG)
			if (level_editor->selectedNum > temp_id)
				level_editor->selectedNum--;
#endif
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
	//obj->AddComponent(new Transform());
	assignIdToObject(obj);
	return obj;
}

//Creates an empty prefab with no components
Object* Factory::createEmptyPrefab()
{
	Object* obj = new Object();
	obj->ObjectId = -1;
	obj->AddComponent(new Transform());

	return obj;
}

//This gives a game object an ID tag which can be used to find that same game object
//Also sets the player object if the object is player controllable
void Factory::assignIdToObject(Object* gameObj)
{
	gameObj->ObjectId = nextObjectId;
	objectMap[nextObjectId] = gameObj;
	++nextObjectId;

	if (gameObj->GetComponent(ComponentType::PlayerControllable)) {
		GameLogic::playerObj = gameObj;
	}
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
		return nullptr;
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
Object* Factory::cloneObject(Object* object, float posoffsetx, float posoffsety)
{
	//Object* obj = createEmptyObject();
	Object* obj = new Object();

	// Clone the object name and the prefab it is using
	obj->name = object->GetName();
	obj->usingPrefab = object->usingPrefab;

	// Clone all components (specific copying might be better to be at their component itself)
	for (const std::pair<ComponentType, Component*>& component : object->Components)
	{
		// Copy transform data
		if (component.first == ComponentType::Transform)
		{
			Transform* trans = (Transform*)((ComponentCreator<Transform>*) componentMap["Transform"])->Create();
			Transform* tran_pt = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));

			trans->Position.x = tran_pt->Position.x + posoffsetx;
			trans->Position.y = tran_pt->Position.y + posoffsety;

			trans->PrevPosition = tran_pt->PrevPosition;

			trans->Scale = tran_pt->Scale;
			trans->Rotation = tran_pt->Rotation;

			obj->AddComponent(trans);
		}

		// Copy texture data
		else if (component.first == ComponentType::Texture)
		{
			Texture* tex = (Texture*)((ComponentCreator<Texture>*) componentMap["Texture"])->Create(std::string());
			Texture* tex_pt = static_cast<Texture*>(object->GetComponent(ComponentType::Texture));

			tex->textureName = tex_pt->textureName;
			tex->opacity = tex_pt->opacity;
			obj->AddComponent(tex);
		}
		// Clone body data
		else if (component.first == ComponentType::Body)
		{
			Body* body_pt = static_cast<Body*>(object->GetComponent(ComponentType::Body));

			if (body_pt->GetShape() == Shape::Rectangle) {
				Rectangular* r = (Rectangular*)((ComponentCreator<Rectangular>*) componentMap["Rectangle"])->Create();

				r->collision_response = ((Rectangular*)body_pt)->collision_response;
				r->width = ((Rectangular*)body_pt)->width;
				r->height = ((Rectangular*)body_pt)->height;
				r->aabb = ((Rectangular*)body_pt)->aabb;
				r->collision_flag = ((Rectangular*)body_pt)->collision_flag;
				r->active = ((Rectangular*)body_pt)->active;
				r->pushable = ((Rectangular*)body_pt)->pushable;
				r->material = ((Rectangular*)body_pt)->material;

				obj->AddComponent(r);
			}
			else if (body_pt->GetShape() == Shape::Circle) {
				Circular* c = (Circular*)((ComponentCreator<Circular>*) componentMap["Circular"])->Create();

				c->collision_response = ((Circular*)body_pt)->collision_response;
				c->circle = ((Circular*)body_pt)->circle;

				obj->AddComponent(c);
			}
			else if (body_pt->GetShape() == Shape::Line) {
				Lines* l = (Lines*)((ComponentCreator<Lines>*) componentMap["Line"])->Create();

				l->collision_response = ((Lines*)body_pt)->collision_response;
				l->line = ((Lines*)body_pt)->line;

				obj->AddComponent(l);
			}
		}
		// Clone physics data
		else if (component.first == ComponentType::Physics)
		{
			Physics* p = (Physics*)((ComponentCreator<Physics>*) componentMap["Physics"])->Create();
			Physics* p_pt = static_cast<Physics*>(object->GetComponent(ComponentType::Physics));

			p->Velocity = p_pt->Velocity;
			p->Mass = p_pt->Mass;
			p->AffectedByGravity = p_pt->AffectedByGravity;
			p->AbleToPushObjects = p_pt->AbleToPushObjects;

			obj->AddComponent(p);
		}
		// Clone player controllable data
		else if (component.first == ComponentType::PlayerControllable)
		{
			PlayerControllable* p = (PlayerControllable*)((ComponentCreator<PlayerControllable>*) componentMap["Player"])->Create();
			obj->AddComponent(p);
		}
		// Clone Animations data
		else if (component.first == ComponentType::Animation) // @guochen
		{
			Animation* ani = (Animation*)((ComponentCreator<Animation>*) componentMap["Animation"])->Create();
			Animation* ani_tmp = static_cast<Animation*>(object->GetComponent(ComponentType::Animation));

			ani->animation_tex_obj = ani_tmp->animation_tex_obj;
			ani->animation_Map = ani_tmp->animation_Map;
			ani->frame_rate = ani_tmp->frame_rate;
			ani->jump_fixed_frame = ani_tmp->jump_fixed_frame;
			ani->opacity = ani_tmp->opacity;
			ani->face_right = ani_tmp->face_right;
			ani->animation_scale = ani_tmp->animation_scale;
			ani->animation_frame = ani_tmp->animation_frame;



			obj->AddComponent(ani);
		}
		else if (component.first == ComponentType::Event)
		{
			Event* e = (Event*)((ComponentCreator<Event>*) componentMap["Event"])->Create();
			Event* e_tmp = static_cast<Event*>(object->GetComponent(ComponentType::Event));


			e->linked_event = e_tmp->linked_event;

			obj->AddComponent(e);
		}
		else if (component.first == ComponentType::Behaviour)
		{
			Behaviour* b = (Behaviour*)((ComponentCreator<Behaviour>*) componentMap["Behaviour"])->Create();
			Behaviour* b_tmp = static_cast<Behaviour*>(object->GetComponent(ComponentType::Behaviour));
			obj->AddComponent(b);
			b->SetBehaviourIndex(b_tmp->GetBehaviourIndex());
			b->SetBehaviourName(b_tmp->GetBehaviourName());
			b->SetBehaviourCounter(b_tmp->GetBehaviourCounter());
			b->SetBehaviourDistance(b_tmp->GetBehaviourDistance());
			b->SetBehaviourSpeed(b_tmp->GetBehaviourSpeed());
		}
		else if (component.first == ComponentType::Text)
		{
			Text* t = (Text*)((ComponentCreator<Text>*) componentMap["Text"])->Create();
			Text* t_tmp = static_cast<Text*>(object->GetComponent(ComponentType::Text));

			t->text = t_tmp->text;
			t->fontSize = t_tmp->fontSize;

			obj->AddComponent(t);
		}
		else if (component.first == ComponentType::Dialogue)
		{
			Dialogue* d = (Dialogue*)((ComponentCreator<Dialogue>*) componentMap["Dialogue"])->Create();
			Dialogue* d_tmp = static_cast<Dialogue*>(object->GetComponent(ComponentType::Dialogue));
			d->ChangeDialogue(d_tmp->GetEntireDialogue());
			obj->AddComponent(d);

		}
	}

	obj->Initialize();
	return obj;
}

//This adds a new component creator which is necessary for the creation of game objects
//Call this at the very start of the game loop in Initialize
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

std::vector<Object*> Factory::FindAllObjectsByName(std::string name)
{

	std::vector<Object*> v;

	if (objectMap.empty())
	{
		return v;
	}

	for (auto it = objectMap.begin(); it != objectMap.end(); it++)
	{
		Object* testObject = it->second;
		std::string objectName = testObject->GetName();
		if (objectName == name)
		{
			v.push_back(testObject);
		}
	}

	return v;
}

//This will delete the specified component from a given object
void Factory::DeleteComponent(int id, ComponentType c) {
	DeleteComponent(getObjectWithID(id), c);
}

void Factory::DeleteComponent(Object* o, ComponentType c) {
	delete o->Components[c];
	o->Components.erase(c);
}

int Factory::CreateLayer(std::string layerName, bool isVisible, bool static_layer) {
	return CreateLayer(layerName, { isVisible, static_layer });
}

int Factory::CreateLayer(std::string layerName, LayerSettings settings) {

	std::pair<LayerSettings, std::vector<Object*>> innerPair = std::make_pair(settings, std::vector<Object*>());

	// Create the outer pair with the layer name and the inner pair
	std::pair<std::string, std::pair<LayerSettings, std::vector<Object*>>> layer = std::make_pair(layerName, innerPair);

	// Add the newly created pair to the layers vector
	SceneManager::layers.push_back(layer);

	return static_cast<int>(SceneManager::layers.size() - 1);
}

void Factory::AddToLayer(int layerNum, Object* obj) {

	// Check if out of bounds
	if (layerNum < SceneManager::layers.size()) {
		// If not out of bounds, push the object pointer
		SceneManager::layers[layerNum].second.second.push_back(obj);
	}
}

void Factory::AddToLayer(std::string layerName, Object* obj) {
	for (auto& layer : SceneManager::layers) {
		if (layer.first == layerName) {
			layer.second.second.push_back(obj);
			return;
		}
	}
}

int Factory::GetLayerNum(std::string layerName) {
	auto it = std::find_if(SceneManager::layers.begin(), SceneManager::layers.end(), [&layerName](const auto& layer) {
		return layer.first == layerName;
		});

	if (it != SceneManager::layers.end()) {
		return static_cast<int>(it - SceneManager::layers.begin());
	}
	else {
		return -1;
	}
}

std::pair<std::string, std::pair<LayerSettings, std::vector<Object*>>>* Factory::GetLayer(std::string layerName)
{
	for (auto& l : SceneManager::layers) {
		if (l.first == layerName)
			return &l;
	}

	return nullptr;
}

std::pair<std::string, std::pair<LayerSettings, std::vector<Object*>>>* Factory::GetLayer(int layerNum)
{
	return &SceneManager::layers[layerNum];
}

std::pair<std::string, std::pair<LayerSettings, std::vector<Object*>>>* Factory::FindLayerThatHasThisObject(Object* obj) {
	for (auto& l : SceneManager::layers) {
		std::vector<Object*>& v = l.second.second;
		auto it = std::find(v.begin(), v.end(), obj);
		if (it != v.end()) {
			return &l;
		}
	}
	return nullptr;
}