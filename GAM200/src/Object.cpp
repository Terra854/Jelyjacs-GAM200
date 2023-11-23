/* !
@file	Object.cpp
@author	Yeo Jia Ming
@date	28/9/2023

This file contains the implementation for the game objects.
Contains member functions to add components into an object, 
and retrieve a specific component from the object.
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Object.h"
#include <components/Behaviour.h> // FOR TESTING PURPOSES, REMOVE LATER
#include <iostream> // FOR TESTING PURPOSES, REMOVE LATER

Object::Object(){}

Object::~Object(){
	// Delete all components inside the object
	for (const std::pair<ComponentType, Component*>& c : Components) {
		delete c.second;
	}
	Components.clear();
}

Component* Object::GetComponent(ComponentType typeID)
{
	/*
	//loop through component map to find the game component
	for (const std::pair<ComponentType, Component*>& c : Components) {
		if (c.first == typeID)
			return c.second;
	}
	*/
	if(Components.find(typeID) != Components.end())
		return (Components.find(typeID))->second;
	else
		return nullptr;
}

void Object::AddComponent(Component* component)
{
	//add into component map
	Components[component->TypeId()] = component;

	if(component->TypeId() == ComponentType::Behaviour)
	{
		Behaviour* behaviour = static_cast<Behaviour*>(component);
		std::cout << "Object name : " << this->name << "      |     Behaviour added in Obj: " << behaviour->behaviour_name << std::endl;
	}

	//sets the component's base ptr to refer to the current object that it is apart of
	component->Base = this;
}

void Object::Initialize()
{
	for (std::unordered_map<ComponentType, Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
	{
		(*it).second->Base = this;
		(*it).second->Initialize();
	}
}