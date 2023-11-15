/* !
@file LogicScript.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration of the functions that are part of the LogicScript Interface
*//*__________________________________________________________________________*/
#pragma once
#include <string>
#include <Object.h>
class LogicScript
{
public:
	LogicScript() {};
	//LogicScript(std::string name);
	~LogicScript() = default;
	
	virtual void Start(Object* obj) = 0 ;
	virtual void Update(Object* obj) = 0;
	virtual void Shutdown(Object* obj) = 0;
};