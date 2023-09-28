#pragma once
/* !
@file	GameLogic.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Game Logic system class
*//*__________________________________________________________________________*/
#include <Debug.h>

#include "Interface_System.h"


class GameLogic : public ISystems
{
public:
	virtual void MessageRelay(Message_Handler* msg);
	virtual void Initialize();
	void Update(float time);
	virtual std::string SystemName() { return "Game_Logic"; }
};

extern GameLogic* Logic;



