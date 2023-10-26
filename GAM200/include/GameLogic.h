#pragma once
/* !
@file GameLogic.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Game Logic system class
*//*__________________________________________________________________________*/
#include <Debug.h>

#include "Interface_System.h"


class GameLogic : public ISystems
{
public:
	//Constructor to initialise pointer
	// It's broken, will be purged in the future
	//GameLogic();
	//~GameLogic();
	virtual void MessageRelay(Message_Handler* msg);
	virtual void Initialize();
	void Update();
	virtual std::string SystemName() { return "Game_Logic"; }
private:

	//std::vector<BehaviourFCT> behaviours;
};

extern GameLogic* Logic;



