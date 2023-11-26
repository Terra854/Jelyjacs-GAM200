/* !
@file Behaviour.h
@author Luke Goh
@date	3/11/2023

This file contains the Behaviour Component that is needed for Logic System
*//*__________________________________________________________________________*/
#pragma once

#include <Object.h>
#include <iostream>

// Behaviour Component
class Behaviour : public Component								
{
public:
	Behaviour() : Component(), behaviour_index{ 999 }, behaviour_name{"NULL"} {}
	Behaviour(int index, std::string name) : Component(), behaviour_index{ index }, behaviour_name{ name } {}

	~Behaviour() = default;
	virtual void Initialize() override {}

	void SetBehaviourIndex(int index) { 
		behaviour_index = index; 
		
	}	// Set the index of the behaviour
	int GetBehaviourIndex() noexcept { return behaviour_index; }			// Get the index of the behaviour

	void SetBehaviourName(std::string name) { 
		behaviour_name = name; 
		std::cout << "Behaviour Name Set : " << behaviour_name << " for " <<  GetOwner()->GetName() << " with ID " << GetOwner()->GetId() << std::endl;
	}
	std::string GetBehaviourName() noexcept { return behaviour_name; }
	
	virtual ComponentType TypeId() const override { return ComponentType::Behaviour; }

private:
	int behaviour_index;
	std::string behaviour_name;
};