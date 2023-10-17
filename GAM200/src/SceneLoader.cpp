/* !
@file	SceneLoader.cpp
@author	Tay Sen Chuan
@date	

This file contains the definitions 
*//*__________________________________________________________________________*/
#include "SceneLoader.h"
#include <fstream>
#include <iostream>

void LoadScene(std::string filename)
{
	// Check if the given file exists
	std::ifstream jsonFile(filename);
	if (!jsonFile.is_open()) {
		std::cerr << "Could not load scene file: " << filename << std::endl;
		return;
	}

	Json::Value* jsonObject = new Json::Value;;
	Json::Reader reader;

	// Check if the given file is a valid json file
	if (!reader.parse(jsonFile, *jsonObject)) {
		std::cout << "Failed to parse JSON" << std::endl;
		jsonFile.close();
		return;
	}
	jsonFile.close();

	// Now parse the file to populate the object with components

	std::string levelname = (*jsonObject)["SceneName"].asString();
	std::cout << "Loading Scene: " << levelname << std::endl;

	for (const auto& component : (*jsonObject)["Objects"]) {

		std::string objprefabs = component["Prefabs"].asString();
		//WIP

		/*



		if (objprefabs == "Transform") {

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
			std::map<std::string, GLuint>::iterator it = textures.find(component["Properties"]["texturepath"].asString());
			if (it == textures.end())
				std::cout << "Missing Texture!" << std::endl;
			else
			{
				GLuint texturepath = it->second;
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


	std::cout << "Background" << std::endl;
	testObj = objectFactory->createObject("../Asset/Objects/background.json");
	scale_and_rotate = objectFactory->createObject("../Asset/Objects/scale-and-rotate.json");
	//std::cout << "test Object 2" << std::endl;
	//testObj2 = objectFactory->createObject("../drop-forever.json");
	floor1 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	floor2 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	floor3 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	Transform* tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Transform));

	// offset objects
	tran_pt->Position.x = 190;

	// @Sen Chuan if you move position like that, remember to call these 2 functions cause collision
	// box still using the old position
	Rectangular* rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Body)); // @Sen Chuan
	rect_pt->Initialize(); // @Sen Chuan

	tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Transform)); // change to different obj
	tran_pt->Position.x = 380;
	tran_pt->Position.y = -300;

	rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Body));
	rect_pt->Initialize();

	std::cout << "test Player" << std::endl;
	playerObj = objectFactory->createObject("../Asset/Objects/player.json");

	// Floor
	for (int i = 0; i < 44; i++) {
		Object* floor = objectFactory->createObject("../Asset/Objects/mapbox.json");
		Transform* floor_t = static_cast<Transform*>(floor->GetComponent(ComponentType::Transform));
		floor_t->Position = { -1100.0f + (static_cast<Transform*>(floor->GetComponent(ComponentType::Transform))->Scale_x * (float)i), -500.0f };
		Rectangular* floor_b = static_cast<Rectangular*>(floor->GetComponent(ComponentType::Body)); // @Sen Chuan
		floor_b->Initialize();
	}

	// Dynamic collision
	dynamic_collision = objectFactory->createObject("../Asset/Objects/mapbox.json");
	Transform* dynamic_collision_t = static_cast<Transform*>(dynamic_collision->GetComponent(ComponentType::Transform));
	dynamic_collision_t->Position = { -1000.0f , -446.0f };
	Rectangular* dynamic_collision_b = static_cast<Rectangular*>(dynamic_collision->GetComponent(ComponentType::Body));
	dynamic_collision_b->Initialize();
	Physics* p = new Physics();
	dynamic_collision->AddComponent(p);
	*/
	}
}













