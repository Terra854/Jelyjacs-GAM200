#pragma once
/* !
@file GameLogic.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Game Logic system class
*//*__________________________________________________________________________*/
#include <Interface_System.h>
#include <Debug.h>
#include <LogicScript.h>
#include <components/Behaviour.h>


class GameLogic : public ISystems
{
public:
	//Constructor to initialise pointer
	GameLogic();
	~GameLogic();
	virtual void MessageRelay(Message_Handler* msg);
	virtual void Initialize();
	void Update();
	void AddBehaviour(std::string name, LogicScript* behaviour);
	void AddObject(Object* obj);
	virtual std::string SystemName() { return "Game_Logic"; }

private:
	std::unordered_map<std::string, LogicScript*> behaviours;
	std::vector<Object*> b_objects;
	std::vector<Behaviour*> behaviourComponents;
};

extern GameLogic* Logic;



