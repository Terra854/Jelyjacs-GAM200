/* !
@file LogicScript.h
@author Luke Goh
@date	3/11/2023

This file contains the declaration of the Test script that are part of the LogicScript Interface
*//*__________________________________________________________________________*/
#pragma once
#include <string>
#include <Object.h>
#include "GameLogic.h"
#include <iostream>

std::map<std::string, LogicScript*> temp_scriptmap;
namespace Script1 {
	class Test : public LogicScript {
	public:
		Test(std::string name);

		void Start(Object* obj);
		void Update(Object* obj);
		void Shutdown(Object* obj);
	};
}