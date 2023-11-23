#pragma once
/* !
@file GameLogic.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Game Logic system class
*//*__________________________________________________________________________*/
#include <Interface_System.h>
#include <Debug.h>
#include <components/Behaviour.h>
#include <map>
#include <iostream>

class LogicScript;
extern std::map<std::string, LogicScript*> temp_scriptmap;

class GameLogic : public ISystems
{
public:
	//Constructor to initialise pointer
	// It's broken, will be purged in the future
	GameLogic();
	~GameLogic();
	virtual void MessageRelay(Message_Handler* msg);
	virtual void Initialize();
	void Update();
	//void Exit();
	void AddBehaviour(std::string name, LogicScript* behaviour);
	void AddBehaviourComponent(Behaviour* behaviour) {
		behaviourComponents.push_back(behaviour);
	}
	bool CheckBehaviour(std::string name);
	//void AddObject(Object* obj);
	void LoadScripts();
	virtual std::string SystemName() { return "Game_Logic"; }

	static Object* playerObj;

private:
	std::unordered_map<std::string, LogicScript*> behaviours; // Map of behaviours to be added to objects on creation 
	std::vector<Behaviour*> behaviourComponents; // Vector of behaviours to be updated every frame 


};

extern GameLogic* Logic;

class LogicScript
{
public:
	LogicScript() {};
	LogicScript(std::string name) {
		temp_scriptmap[name] = this;
		std::cout << "LogicScript Map size : " << temp_scriptmap.size() << std::endl;
	}
	virtual ~LogicScript() {};

	virtual void Start(Object* obj) = 0;
	virtual void Update(Object* obj) = 0;
	virtual void Shutdown(Object* obj) = 0;
};

