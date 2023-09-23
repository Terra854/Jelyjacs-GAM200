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
	for (; it != componentMap.end(); ++it)
	{
		delete it->second;
	}
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
		return NULL;
	}

	// Now parse the file to populate the object with components

	Object* obj = new Object();

	for (const auto& component : (*jsonObject)["Components"]) {
		std::string type = component["Type"].asString();

		if (type == "Transform") {

			Transform* trans = (Transform*)((ComponentCreator<Transform>*) componentMap["Transform"])->Create();

			trans->Position.x = component["Properties"]["Position"]["x"].asFloat();
			trans->Position.y = component["Properties"]["Position"]["y"].asFloat();

			trans->PrevPosition = trans->Position;

			trans->Scale = component["Properties"]["Scale"].asFloat();
			trans->Rotation = component["Properties"]["Rotation"].asFloat();

			trans->Matrix = Mat3Scale(trans->Scale, trans->Scale) * Mat3RotDeg(trans->Rotation) * Mat3Translate(trans->Position.x, trans->Position.y);

			obj->AddComponent(trans);
		}
		else if (type == "Texture") {
			GLuint texturepath = app->setup_texobj(component["Properties"]["texturepath"].asCString());
			Texture* tex = (Texture*)((ComponentCreator<Texture>*) componentMap["Texture"])->Create(texturepath);
			obj->AddComponent(tex);
		}
		else if (type == "Body") {
			std::string shape = component["Shape"].asString();
			if (shape == "Rectangle") {
				Rectangular* r = (Rectangular*)((ComponentCreator<Rectangular>*) componentMap["Rectangle"])->Create();

				r->aabb.min.x = component["Properties"]["AABB"]["min"]["x"].asFloat();
				r->aabb.min.y = component["Properties"]["AABB"]["min"]["y"].asFloat();
				r->aabb.max.x = component["Properties"]["AABB"]["max"]["x"].asFloat();
				r->aabb.max.y = component["Properties"]["AABB"]["max"]["y"].asFloat();

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

			p->X_Velocity = component["Properties"]["X_Velocity"].asFloat();
			p->X_Acceleration = component["Properties"]["X_Acceleration"].asFloat();
			p->Y_Velocity = component["Properties"]["Y_Velocity"].asFloat();
			p->Y_Acceleration = component["Properties"]["Y_Acceleration"].asFloat();
			p->Mass = component["Properties"]["Mass"].asFloat();

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
void Factory::Update(float dt) {
	std::set<Object*>::iterator it = gameObjsToBeDeleted.begin();
	for (; it != gameObjsToBeDeleted.end(); ++it)
	{
		Object* obj = *it;
		objectIDMap::iterator it = objectMap.find(obj->ObjectId);
		if (it != objectMap.end())
		{
			//Delete it and remove its entry in the Id map
			delete obj;
			objectMap.erase(it);
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
	Object* obj = new Object();
	assignIdToObject(obj);
	return obj;
}

//  This creates an object from a text file. This is old code
//  Will be purged at some point

/*
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
/*
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
}*/

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
void Factory::AddComponentCreator(const std::string& name, BaseComponentCreator* creator)
{
	componentMap[name] = creator;
}