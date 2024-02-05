#pragma once
/* !
@file GameLogic.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Game Logic system class & LogicScript class
*//*__________________________________________________________________________*/
#include <Interface_System.h>
#include <Debug.h>
#include <components/Behaviour.h>
#include <map>
#include <iostream>

class LogicScript;


class GameLogic : public ISystems
{
public:
	// Constructor and Destructor for the GameLogic class.
	GameLogic();
	~GameLogic();

	// Override of the MessageRelay method from ISystems.
	// @param msg: A pointer to the Message_Handler to handle messages.				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));

	virtual void MessageRelay(Message_Handler* msg);

	// Initialize method for setting up the GameLogic.
	virtual void Initialize();

	// Update method called every frame to update the state of the logic.
	void Update();
	//void Exit();

	// Method for adding a new behavior script to the game logic.
	// @param name: The name of the behavior.
	// @param behaviour: Pointer to the LogicScript representing the behavior.
	void AddBehaviour(std::string name, LogicScript* behaviour);

	// Method for adding a behavior component to the update list.
	// @param behaviour_comp: Pointer to the Behaviour component to be added.
	void AddBehaviourComponent(Behaviour* behaviour_comp) {
		behaviourComponents.push_back(behaviour_comp);
		std::cout << "Added Behaviour Component from "  << behaviour_comp->GetOwner()->GetName() << std::endl;
	}

	// Method for removing a behavior component from the update list.
	// @param behaviour: Pointer to the Behaviour component to be removed.
	void RemoveBehaviourComponent(Behaviour* behaviour) {
		for (int i = 0; i < behaviourComponents.size(); i++) {
			if (behaviourComponents[i]->GetOwner()->GetId() == behaviour->GetOwner()->GetId()) {
				behaviourComponents.erase(behaviourComponents.begin() + i);
				std::cout << "Erased Behaviour Component" << std::endl;
			}
		}
	}

	// Method to check if a behavior exists.
	// @param name: The name of the behavior to check.
	bool CheckBehaviour(std::string name);

	//void AddObject(Object* obj);

	// Method for loading and initializing scripts.
	void LoadScripts();

	// Override of the SystemName method to return the name of this system.
	virtual std::string SystemName() { return "Game_Logic"; }

	// Static pointer to the player object, accessible globally.
	static Object* playerObj;

	// Map of behaviors to be added to objects on creation.
	std::unordered_map<std::string, LogicScript*> behaviours; 

	// Vector of behaviourComponents to be updated every frame 
	std::vector<Behaviour*> behaviourComponents; 


};

extern GameLogic* Logic;
extern int CatPower;

class LogicScript
{
public:
	// Friend class declaration to allow GameLogic to access private members of LogicScript.
	friend class GameLogic;

	// Constructors and Destructor for the LogicScript class.
	LogicScript() {};
	LogicScript(std::string name) {
		temp_scriptmap[name] = this;
		std::cout << "LogicScript Map size : " << temp_scriptmap.size() << std::endl;
	}
	virtual ~LogicScript() {};

	// Pure virtual methods defining the lifecycle of a script.
	// These methods must be implemented by derived classes.
	virtual void Start(Object* obj) = 0;
	virtual void Update(Object* obj) = 0;
	virtual void Shutdown(Object* obj) = 0;
private:
	// Static map to hold temporary script mappings.
	static std::map<std::string, LogicScript*> temp_scriptmap;
};

