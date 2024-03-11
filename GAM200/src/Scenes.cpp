/* !
@file	Scenes.cpp
@author	Tay Sen Chuan
@date	1/11/2023

This file contains the definitions for loading and saving scenes
*//*__________________________________________________________________________*/

#include <Debug.h>
#include "Assets Manager/asset_manager.h"
#include "Assets Manager/json_serialization.h"
#include "../include/components/Event.h"
#include "Scenes.h"
#include <iostream>
#include "LevelEditor.h"
#include <SceneManager.h>
#include <components/Animation.h>
#include <components/Text.h>
#include <Factory.h>
#include <components/Particle.h>

/******************************************************************************
LoadSceneFromJson
-	This function reads a json file and loads all the objects

@param filename - the name of the json file
*******************************************************************************/
void LoadSceneFromJson(std::string filename, bool isParentScene)
{
	// Check if the given file exists
	JsonSerialization jsonobj;
	jsonobj.openFileRead("Asset/Levels/" + filename);

	std::string levelname;
	jsonobj.readString(levelname, "SceneName");
	std::cout << "Loading Scene: " << levelname << std::endl;

	//AssetManager::clearSoundMap();
	/*if (jsonobj.isMember("SoundMap"))
	{
		std::cout << "Linking sound map..." << std::endl;
		std::string soundmap;
		jsonobj.readString(soundmap, "SoundMap");
		linkSoundMap(soundmap);
	}*/

	/*
	std::string soundmap = "Asset/Sounds/sounds.json";
	linkSoundMap(soundmap);

	audio->setupSound();
	*/

	if (isParentScene) {
		SceneManager::AdditionalScenesLoadedConcurrently.clear();

		Vec2 start_coord;
		jsonobj.readFloat(start_coord.x, "Size", "startX");
		jsonobj.readFloat(start_coord.y, "Size", "startY");
		engine->Set_Start_Coords(start_coord);

		Vec2 level_size;
		jsonobj.readFloat(level_size.x, "Size", "width");
		jsonobj.readFloat(level_size.y, "Size", "height");
		engine->Set_Level_Size(level_size);
	}

	int layerNum = 0;

	for (auto& layer : jsonobj.read("Layers")) {
		std::string layername = layer["Name"].asCString();

		//jsonobj.readString(layername, "Name");
		LayerSettings ls = { layer["Settings"]["isVisible"].asBool(), layer["Settings"]["static_layer"].asBool(), !isParentScene };

		if (!isParentScene)
			ls.inheritedJsonName = levelname;

		objectFactory->CreateLayer(layername, ls);

		//for (auto& component : jsonobj.read("Objects"))
		/*
		for (auto& component : layer["Objects"])
		{
			JsonSerialization jsonloop;
			jsonloop.jsonObject = &component;

			std::string objprefabs;
			jsonloop.readString(objprefabs, "Prefabs");
			Object* obj;
			// Create the prefab if it doesn't exist
			if (!objprefabs.empty() && AssetManager::prefabsval(objprefabs) == nullptr)
			{
				// Create new prefab
				std::string tempobjprefabs = AssetManager::objectprefabsval() + "/" + objprefabs;
				Object* newPrefab = objectFactory->createObject(tempobjprefabs);

				AssetManager::updateprefab(newPrefab->GetName(), newPrefab);
			}

			if (AssetManager::prefabsval(objprefabs)) {
				// Create object via cloning the prefab
				obj = objectFactory->cloneObject(AssetManager::prefabsval(objprefabs));
				obj->SetPrefab(AssetManager::prefabsval(objprefabs)); // Update the clone object to have usingPrefab value
				// Assign an ID. It will be added to the objectMap
				objectFactory->assignIdToObject(obj);
			}
			else {
				// Create object via creating a new object
				obj = objectFactory->createEmptyObject();
			}

			// Read extra data to update object
			std::string type;
			jsonloop.readString(type, "Type");

			if (type == "Transform")
			{
				Transform* tran_pt = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
				jsonloop.readFloat(tran_pt->Position.x, "Position", "x");
				jsonloop.readFloat(tran_pt->Position.y, "Position", "y");
				jsonloop.readFloat(tran_pt->Scale.x, "Scale", "x");
				jsonloop.readFloat(tran_pt->Scale.y, "Scale", "y");
				jsonloop.readFloat(tran_pt->Rotation, "Rotation");
			}

			if (jsonloop.isMember("Name"))
			{
				std::string tempstr;
				jsonloop.readString(tempstr, "Name");
				obj->SetName(tempstr);
			}
			//else
			//	obj->SetName("");
			// ^Default name for objects is the name set in prefabs object list

			if (jsonloop.isMember("Properties"))
			{
				Body* temp = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
				if (temp->GetShape() == Shape::Rectangle)
				{
					Rectangular* temp2 = static_cast<Rectangular*>(temp);
					jsonloop.readFloat(temp2->width, "Properties", "width");
					jsonloop.readFloat(temp2->height, "Properties", "height");
				}
			}

			if (jsonloop.isMember("linkedevent"))
			{
				Event* event_pt = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
				jsonloop.readInt(event_pt->linked_event, "linkedevent");
			}

			if (jsonloop.isMember("scripts"))
			{
				//// THIS DOESN'T REALLY WORK FOR CHANGING THE SCRIPTS OF INDIVIDUAL OBJECTS
				//std::string behvstr;
				//jsonloop.readString(behvstr, "scripts");

				////Behaviour* b = (Behaviour*)(obj->GetComponent(ComponentType::Behaviour));
				////Logic->RemoveBehaviourComponent(b);
				//objectFactory->DeleteComponent(obj, ComponentType::Behaviour);

				//obj->AddComponent(new Behaviour(0, behvstr));
				////Logic->AddBehaviourComponent((Behaviour*)obj->GetComponent(ComponentType::Behaviour));

				//Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));

				//behv->SetBehaviourName(behvstr);
			}

			if (jsonloop.isMember("index"))
			{
				int index{};
				jsonloop.readInt(index, "index");

				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
				behv->SetBehaviourIndex(index);
			}

			if (jsonloop.isMember("counter"))
			{
				float counter{};
				jsonloop.readFloat(counter, "counter");

				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
				behv->SetBehaviourCounter(counter);
			}

			if (jsonloop.isMember("speed"))
			{
				float speed{};
				jsonloop.readFloat(speed, "speed");

				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
				behv->SetBehaviourSpeed(speed);
			}

			if (jsonloop.isMember("distance"))
			{
				float distance{};
				jsonloop.readFloat(distance, "distance");

				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
				behv->SetBehaviourDistance(distance);
			}

			// Add here to read oher types of data if necessary WIP


			obj->Initialize();

			objectFactory->AddToLayer(layername, obj);
		}*/
		for (auto& objects : layer["Objects"])
		{
			JsonSerialization jsonloop;
			jsonloop.jsonObject = &objects;

			std::string objprefabs;
			jsonloop.readString(objprefabs, "Prefabs");
			Object* obj;
			// Create the prefab if it doesn't exist
			if (!objprefabs.empty() && AssetManager::prefabsval(objprefabs) == nullptr)
			{
				// Create new prefab
				std::string tempobjprefabs = AssetManager::objectprefabsval() + "/" + objprefabs;
				Object* newPrefab = objectFactory->createObject(tempobjprefabs);

				AssetManager::updateprefab(newPrefab->GetName(), newPrefab);
			}

			if (AssetManager::prefabsval(objprefabs)) {
				// Create object via cloning the prefab
				obj = objectFactory->cloneObject(AssetManager::prefabsval(objprefabs));
				obj->SetPrefab(AssetManager::prefabsval(objprefabs)); // Update the clone object to have usingPrefab value
				// Assign an ID. It will be added to the objectMap
				objectFactory->assignIdToObject(obj);
			}
			else {
				// Create object via creating a new object
				obj = objectFactory->createEmptyObject();
			}
			for (auto& component : jsonloop.read("Components")) {

				JsonSerialization jsonComponent;
				jsonComponent.jsonObject = &component;

				std::string type;
				jsonComponent.readString(type, "Type");

				if (type == "Transform") {

					Transform* trans = (Transform*)((ComponentCreator<Transform>*) objectFactory->componentMap["Transform"])->Create();

					jsonComponent.readFloat(trans->Position.x, "Properties", "Position", "x");
					jsonComponent.readFloat(trans->Position.y, "Properties", "Position", "y");

					jsonComponent.readFloat(trans->Scale.x, "Properties", "Scale", "x");
					jsonComponent.readFloat(trans->Scale.y, "Properties", "Scale", "y");
					jsonComponent.readFloat(trans->Rotation, "Properties", "Rotation");

					obj->AddComponent(trans);
				}
				else if (type == "Texture") {

					std::string path;
					jsonComponent.readString(path, "Properties", "texturepath");
					bool exist = AssetManager::texturecheckexist(path);

					if (!exist) {
						std::cout << "Missing Texture!" << std::endl;
						// Attempt to add the texture
						AssetManager::addtextures(path);
					}

					Texture* tex = (Texture*)((ComponentCreator<Texture>*) objectFactory->componentMap["Texture"])->Create(path);

					jsonComponent.readFloat(tex->opacity, "Properties", "opacity");
					obj->AddComponent(tex);

				}
				else if (type == "Body") {
					std::string shape;
					jsonComponent.readString(shape, "Shape");
					if (shape == "Rectangle") {
						Rectangular* r = (Rectangular*)((ComponentCreator<Rectangular>*) objectFactory->componentMap["Rectangle"])->Create();

						if (jsonComponent.isMember("Collision Response"))
							jsonComponent.readBool(r->collision_response, "Collision Response");

						if (jsonComponent.isMember("Active"))
							jsonComponent.readBool(r->active, "Active");

						jsonComponent.readFloat(r->width, "Properties", "width");
						jsonComponent.readFloat(r->height, "Properties", "height");
						jsonComponent.readBool(r->pushable, "Properties", "pushable");

						int material = 0;
						jsonComponent.readInt(material, "Properties", "material");
						r->material = static_cast<Material>(material);

						obj->AddComponent(r);
					}
					else if (shape == "Circle") {
						Circular* c = (Circular*)((ComponentCreator<Circular>*) objectFactory->componentMap["Circular"])->Create();

						if (jsonComponent.isMember("Collision Response"))
							jsonComponent.readBool(c->collision_response, "Collision Response");

						jsonComponent.readFloat(c->circle.radius, "Properties", "radius");
						jsonComponent.readFloat(c->circle.center.x, "Properties", "radius", "x");
						jsonComponent.readFloat(c->circle.center.y, "Properties", "radius", "y");

						obj->AddComponent(c);
					}
					else if (shape == "Line") {
						Lines* l = (Lines*)((ComponentCreator<Lines>*) objectFactory->componentMap["Line"])->Create();

						if (jsonComponent.isMember("Collision Response"))
							jsonComponent.readBool(l->collision_response, "Collision Response");

						float x;
						float y;

						jsonComponent.readFloat(x, "Properties", "start", "x");
						jsonComponent.readFloat(y, "Properties", "start", "y");
						l->line.SetPt0({ x, y });

						jsonComponent.readFloat(x, "Properties", "end", "x");
						jsonComponent.readFloat(y, "Properties", "end", "y");
						l->line.SetPt1({ x, y });

						obj->AddComponent(l);
					}
				}
				else if (type == "Physics") {
					Physics* p = (Physics*)((ComponentCreator<Physics>*) objectFactory->componentMap["Physics"])->Create();

					float x;
					float y;

					jsonComponent.readFloat(x, "Properties", "X_Velocity");
					jsonComponent.readFloat(y, "Properties", "Y_Velocity");
					p->Velocity = Vec2(x, y);

					jsonComponent.readFloat(p->Mass, "Properties", "Mass");

					jsonComponent.readBool(p->AffectedByGravity, "Properties", "AffectedByGravity");

					jsonComponent.readBool(p->AbleToPushObjects, "Properties", "AbleToPushObjects");

					obj->AddComponent(p);
				}
				else if (type == "Player") {
					PlayerControllable* p = (PlayerControllable*)((ComponentCreator<PlayerControllable>*) objectFactory->componentMap["Player"])->Create();
					obj->AddComponent(p);
				}
				else if (type == "Animation") // @guochen
				{
					Animation* a = (Animation*)((ComponentCreator<Animation>*) objectFactory->componentMap["Animation"])->Create();

					std::string path;
					jsonComponent.readString(path, "Properties", "texturepath");
					bool exist = AssetManager::animationcheckexist(path);
					if (!exist)
					{
						std::cout << "Missing Animation Texture!" << std::endl;
					}
					else
					{
						a->animation_tex_obj = AssetManager::animationval(path);
					}

					jsonComponent.readFloat(a->frame_rate, "Properties", "framerate");

					if (jsonComponent.isMember("jumpfixframe", "Properties"))
						jsonComponent.readInt(a->jump_fixed_frame, "Properties", "jumpfixframe");

					jsonComponent.readFloat(a->opacity, "Properties", "opacity");

					float col;
					float row;
					jsonComponent.readFloat(col, "Properties", "frame", 0);
					jsonComponent.readFloat(row, "Properties", "frame", 1);

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

						if (jsonComponent.isMember(animationtype, "Properties"))
						{
							float framecol;
							float framerow;
							jsonComponent.readFloat(framecol, "Properties", animationtype, 0);
							jsonComponent.readFloat(framerow, "Properties", animationtype, 1);

							for (int i = 0; i < framecol; i++)
							{
								GLApp::GLModel model = a->setup_texobj_animation(i / col, (framerow - 1) / row, (i + 1) / col, framerow / row, faceright);
								animationmodel.push_back(model);
							}
						}

						a->animation_Map.emplace(frametype, animationmodel);
					}

					obj->AddComponent(a);
				}
				else if (type == "Event")
				{
					Event* e = (Event*)((ComponentCreator<Event>*) objectFactory->componentMap["Event"])->Create();

					jsonComponent.readInt(e->linked_event, "Properties", "linkedevent");

					obj->AddComponent(e);
				}
				else if (type == "Behaviour")
				{
					Behaviour* b = (Behaviour*)((ComponentCreator<Behaviour>*) objectFactory->componentMap["Behaviour"])->Create();
					std::string temp_name;
					int temp_index;
					jsonComponent.readString(temp_name, "Properties", "Script");
					jsonComponent.readInt(temp_index, "Properties", "Index");
					obj->AddComponent(b);
					Logic->AddBehaviourComponent(b);
					std::cout << "Behaviour Script & Index: " << temp_name << ", " << temp_index << std::endl;
					b->SetBehaviourIndex(temp_index);
					b->SetBehaviourName(temp_name);
				}
				else if (type == "Particle") // @guochen
				{
					ParticleSystem* p = (ParticleSystem*)((ComponentCreator<ParticleSystem>*) objectFactory->componentMap["Particle"])->Create();
					jsonComponent.readFloat(p->pos_x_min, "Properties", "posx", 0);
					jsonComponent.readFloat(p->pos_x_max, "Properties", "posx", 1);
					jsonComponent.readFloat(p->pos_y_min, "Properties", "posy", 0);
					jsonComponent.readFloat(p->pos_y_max, "Properties", "posy", 1);
					jsonComponent.readFloat(p->vel_x_min, "Properties", "velx", 0);
					jsonComponent.readFloat(p->vel_x_max, "Properties", "velx", 1);
					jsonComponent.readFloat(p->vel_y_min, "Properties", "vely", 0);
					jsonComponent.readFloat(p->vel_y_max, "Properties", "vely", 1);
					jsonComponent.readFloat(p->life_min, "Properties", "lifetime", 0);
					jsonComponent.readFloat(p->life_max, "Properties", "lifetime", 1);

					std::string path;
					jsonComponent.readString(path, "Properties", "texture");
					bool exist = AssetManager::texturecheckexist(path);

					if (!exist) {
						std::cout << "Missing Texture!" << std::endl;
						// Attempt to add the texture
						AssetManager::addtextures(path);
					}
					else
					{
						p->particle_texture = AssetManager::textureval(path);
					}

					std::cout << "PARTICLE TEXTURE SET: " << p->particle_texture << std::endl;

					obj->AddComponent(p);
				}
				else if (type == "Text")
				{
					Text* t = (Text*)((ComponentCreator<Text>*) objectFactory->componentMap["Text"])->Create();
					jsonComponent.readString(t->text, "Properties", "text");

					if (jsonComponent.isMember("fontSize", "Properties"))
						jsonComponent.readFloat(t->fontSize, "Properties", "fontSize");

					obj->AddComponent(t);
				}
			}
			obj->Initialize();

			objectFactory->AddToLayer(layername, obj);
		}

		layerNum++;
	}

	if (isParentScene) {
		// Save the name of the level to engine to track
		engine->loaded_level = levelname;
		engine->loaded_filename = filename;
	}

	for (auto& additionalScenes : jsonobj.read("AdditionalScenesToLoad"))
	{
		std::string sceneFile = additionalScenes.asCString();
		SceneManager::AdditionalScenesLoadedConcurrently.push_back(sceneFile);
		::LoadSceneFromJson(sceneFile, false);
	}

	jsonobj.closeFile();

	Logic->Initialize();
}

/******************************************************************************
SaveScene
-	This function reads the list of all the objects and save them into a json file

@param filename - the name of the json file
*******************************************************************************/
void SaveScene(std::string filename)
{
	// Save Scene Name
	Json::Value jsonobj;
	jsonobj["SceneName"] = engine->loaded_level;

	//jsonobj["SoundMap"] = "Asset/Sounds/sounds.json"; // Hard coded line, will need to do proper saving

	Vec2 start_coord = engine->Get_Start_Coords();

	Vec2 level_size = engine->Get_Level_Size();

	jsonobj["Size"]["startX"] = start_coord.x;
	jsonobj["Size"]["startY"] = start_coord.y;
	jsonobj["Size"]["width"] = level_size.x;
	jsonobj["Size"]["height"] = level_size.y;

	auto& layers = jsonobj["Layers"];

	for (auto& l : SceneManager::layers) {

		// Do not save the inherited layers
		if (l.second.first.isInherited)
			continue;

		Json::Value layer;

		layer["Name"] = l.first;
		layer["Settings"]["isVisible"] = l.second.first.isVisible;
		layer["Settings"]["static_layer"] = l.second.first.static_layer;

		for (Object* obj : l.second.second) {

			if (obj == nullptr)
				continue;

			// Save object prefabs data
			std::string name = obj->GetName();
			Object* prefab = obj->GetPrefab();
			//std::string prefabname = "MISSINGNAME";
			std::string prefabname;

			if (prefab == nullptr)
				std::cout << "OBJECT: " << name << " is missing usingPrefab!" << std::endl;
			else
				prefabname = prefab->GetName() + ".json";

			Json::Value innerobj;


			innerobj["Name"] = name;
			innerobj["Prefabs"] = prefabname;

			/*
			// Save object transform data
			if (obj->GetComponent(ComponentType::Transform) != nullptr)
			{
				Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
				innerobj["Type"] = "Transform";

				Json::Value position;
				position["x"] = trans->Position.x;
				position["y"] = trans->Position.y;
				innerobj["Position"] = position;

				Json::Value scale;
				scale["x"] = trans->Scale.x;
				scale["y"] = trans->Scale.y;
				innerobj["Scale"] = scale;

				innerobj["Rotation"] = trans->Rotation;
			}

			if (obj->GetComponent(ComponentType::Body) != nullptr)
			{
				Body* temp = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
				if (temp->GetShape() == Shape::Rectangle)
				{
					Rectangular* temp2 = static_cast<Rectangular*>(temp);
					Json::Value properties;
					properties["width"] = temp2->width;
					properties["height"] = temp2->height;
					innerobj["Properties"] = properties;
				}
			}

			// Save objects event data
			if (obj->GetComponent(ComponentType::Event) != nullptr)
			{
				Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
				innerobj["linkedevent"] = event->linked_event;
			}

			if (obj->GetComponent(ComponentType::Behaviour))
			{
				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
				innerobj["scripts"] = behv->GetBehaviourName();
				innerobj["index"] = behv->GetBehaviourIndex();
				innerobj["counter"] = behv->GetBehaviourCounter();
				innerobj["speed"] = behv->GetBehaviourSpeed();
				innerobj["distance"] = behv->GetBehaviourDistance();
			}

			layer["Objects"].append(innerobj);
			(*/

			Transform* tr = (Transform*)obj->GetComponent(ComponentType::Transform);
			Texture* te = (Texture*)obj->GetComponent(ComponentType::Texture);
			Body* bo = (Body*)obj->GetComponent(ComponentType::Body);
			Physics* ph = (Physics*)obj->GetComponent(ComponentType::Physics);
			PlayerControllable* pc = (PlayerControllable*)obj->GetComponent(ComponentType::PlayerControllable);
			Animation* a = (Animation*)obj->GetComponent(ComponentType::Animation);
			Event* e = (Event*)obj->GetComponent(ComponentType::Event);
			Behaviour* be = (Behaviour*)obj->GetComponent(ComponentType::Behaviour);
			Text* t = (Text*)obj->GetComponent(ComponentType::Text);

			// Transform
			if (tr != nullptr) {
				Json::Value transform;
				transform["Type"] = "Transform";
				transform["Properties"]["Position"]["x"] = tr->Position.x;
				transform["Properties"]["Position"]["y"] = tr->Position.y;
				transform["Properties"]["Scale"]["x"] = tr->Scale.x;
				transform["Properties"]["Scale"]["y"] = tr->Scale.y;
				transform["Properties"]["Rotation"] = tr->Rotation;
				innerobj["Components"].append(transform);
			}

			// Texture
			if (te != nullptr) {
				Json::Value texture;
				texture["Type"] = "Texture";
				texture["Properties"]["texturepath"] = te->textureName;
				texture["Properties"]["opacity"] = te->opacity;
				innerobj["Components"].append(texture);

			}

			// Animation 
			if (a != nullptr) {
				Json::Value animation;
				animation["Type"] = "Animation";

				// TODO: It's incomplete

				for (const auto& pair : AssetManager::animations) {
					if (pair.second == a->animation_tex_obj) {
						animation["Properties"]["texturepath"] = pair.first;
						break; // Stop searching after the first match is found
					}
				}

				animation["Properties"]["jumpfixframe"] = a->jump_fixed_frame;
				animation["Properties"]["framerate"] = a->frame_rate;
				//animation["Properties"]["frame"] =
				//animation["Properties"]["idle"] =
				//animation["Properties"]["push"] =
				//animation["Properties"]["jump"] =
				//animation["Properties"]["run"] =
				animation["Properties"]["opacity"] = a->opacity;

				innerobj["Components"].append(animation);

			}

			// Text
			if (t != nullptr) {
				Json::Value text;
				text["Type"] = "Text";
				text["Properties"]["text"] = t->text;
				text["Properties"]["fontSize"] = t->fontSize;
				innerobj["Components"].append(text);
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
				innerobj["Components"].append(body);

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
				innerobj["Components"].append(physics);
			}

			// PlayerControllable
			if (pc != nullptr) {
				Json::Value player;
				player["Type"] = "Player";
				innerobj["Components"].append(player);
			}

			// Event
			if (e != nullptr) {
				Json::Value event;
				event["Type"] = "Event";
				event["Properties"]["linkedevent"] = e->linked_event;
				innerobj["Components"].append(event);
			}

			// Behaviour
			if (be != nullptr) {
				Json::Value behaviour;
				behaviour["Type"] = "Behaviour";
				behaviour["Properties"]["Script"] = be->GetBehaviourName();
				behaviour["Properties"]["Index"] = be->GetBehaviourIndex();
				innerobj["Components"].append(behaviour);
			}

			layer["Objects"].append(innerobj);
		}

		layers.append(layer);
	}

	for (std::string& a : SceneManager::AdditionalScenesLoadedConcurrently)
	{
		jsonobj["AdditionalScenesToLoad"].append(a);
	}

	/*
	for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
	{
		Object* obj = objectFactory->getObjectWithID(static_cast<long>(i));

		if (obj == nullptr)
			continue;

		// Save object prefabs data
		std::string name = obj->GetName();
		Object* prefab = obj->GetPrefab();
		std::string prefabname = "MISSINGNAME";

		if (prefab == nullptr)
			std::cout << "OBJECT: " << name << " is missing usingPrefab!" <<std::endl;
		else
			prefabname = prefab->GetName() + ".json";

		Json::Value innerobj;


		innerobj["Name"] = name;
		innerobj["Prefabs"] = prefabname;

		// Save object transform data
		if (obj->GetComponent(ComponentType::Transform) != nullptr)
		{
			Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
			innerobj["Type"] = "Transform";

			Json::Value position;
			position["x"] = trans->Position.x;
			position["y"] = trans->Position.y;
			innerobj["Position"] = position;

			Json::Value scale;
			scale["x"] = trans->Scale.x;
			scale["y"] = trans->Scale.y;
			innerobj["Scale"] = scale;

			innerobj["Rotation"] = trans->Rotation;
		}

		if (obj->GetComponent(ComponentType::Body) != nullptr)
		{
			Body* temp = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
			if (temp->GetShape() == Shape::Rectangle)
			{
				Rectangular* temp2 = static_cast<Rectangular*>(temp);
				Json::Value properties;
				properties["width"] = temp2->width;
				properties["height"] = temp2->height;
				innerobj["Properties"] = properties;
			}
		}

		// Save objects event data
		if (obj->GetComponent(ComponentType::Event) != nullptr)
		{
			Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
			innerobj["linkedevent"] = event->linked_event;
		}

		if (obj->GetComponent(ComponentType::Behaviour))
		{
			Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
			innerobj["scripts"] = behv->GetBehaviourName();
		}

		jsonobj["Objects"].append(innerobj);
	}
	*/

	// Write file
	std::ofstream outputFile("Asset/Levels/" + filename);
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

	// GETTING ERROR, TEMPORARY NOT USING JSONSERILIZATION.H
	// 
	//JsonSerialization jsonobj;
	//jsonobj.writeData("SceneName", filename);	

	//for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++) 
	//{
	//	Object* obj = objectFactory->getObjectWithID((long)i);

	//	if (obj == nullptr)
	//		continue;

	//	std::string name = obj->GetName() + ".json";
	//	jsonobj.writeArrData("Prefabs", name);

	//	if (obj->GetComponent(ComponentType::Transform) != nullptr)
	//	{
	//		Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
	//		jsonobj.writeArrData("Type", "Transform");
	//		Json::Value position;
	//		position["x"] = trans->Position.x;
	//		position["y"] = trans->Position.y;
	//		jsonobj.writeArrData("Position", position);
	//		Json::Value scale;
	//		scale["x"] = trans->Scale.x;
	//		scale["y"] = trans->Scale.y;
	//		jsonobj.writeArrData("Scale", scale);
	//		jsonobj.writeArrData("Rotation", trans->Rotation);
	//	}

	//	if (obj->GetComponent(ComponentType::Event) != nullptr)
	//	{
	//		Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
	//		jsonobj.writeArrData("linkedevent", event->linked_event);
	//	}

	//	jsonobj.appendToArr();
	//}

	//jsonobj.openFileWrite(filename);
	//jsonobj.closeFile();
}









