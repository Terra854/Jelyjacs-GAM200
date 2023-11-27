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
	GameLogic();
	~GameLogic();
	virtual void MessageRelay(Message_Handler* msg);
	virtual void Initialize();
	void Update();
	//void Exit();
	void AddBehaviour(std::string name, LogicScript* behaviour);
	void AddBehaviourComponent(Behaviour* behaviour_comp) {
		behaviourComponents.push_back(behaviour_comp);
		std::cout << "Added Behaviour Component from "  << behaviour_comp->GetOwner()->GetName() << std::endl;
	}
	void RemoveBehaviourComponent(Behaviour* behaviour) {
		for (int i = 0; i < behaviourComponents.size(); i++) {
			if (behaviourComponents[i]->GetOwner()->GetId() == behaviour->GetOwner()->GetId()) {
				behaviourComponents.erase(behaviourComponents.begin() + i);
				std::cout << "Erased Behaviour Component" << std::endl;
			}
		}
	}
	bool CheckBehaviour(std::string name);
	//void AddObject(Object* obj);
	void LoadScripts();

	// Returns name of the component
	virtual std::string SystemName() { return "Game_Logic"; }

	static Object* playerObj;


	std::unordered_map<std::string, LogicScript*> behaviours; // Map of behaviours to be added to objects on creation 
	std::vector<Behaviour*> behaviourComponents; // Vector of behaviours to be updated every frame 


};

extern GameLogic* Logic;

class LogicScript
{
public:
	friend class GameLogic;
	LogicScript() {};
	LogicScript(std::string name) {
		temp_scriptmap[name] = this;
		std::cout << "LogicScript Map size : " << temp_scriptmap.size() << std::endl;
	}
	virtual ~LogicScript() {};

	virtual void Start(Object* obj) = 0;
	virtual void Update(Object* obj) = 0;
	virtual void Shutdown(Object* obj) = 0;
private:
	static std::map<std::string, LogicScript*> temp_scriptmap;
};

