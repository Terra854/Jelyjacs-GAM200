/* !
@file Behaviour.h
@author Luke Goh
@date	3/11/2023

This file contains the Behaviour Component that is needed for Logic System
*//*__________________________________________________________________________*/
#pragma once

#include <Object.h>

// Behaviour Component
class Behaviour : public Component								
{
public:
	Behaviour() : Component(), behaviour_index{ 0 } {}
	Behaviour(const int& index, Object* o) : Component(), behaviour_index{ index } {}

	~Behaviour() = default;
	virtual void Initialize() override {}

	void SetBehaviourIndex(const int& index) { behaviour_index = index; }	// Set the index of the behaviour
	int GetBehaviourIndex() noexcept { return behaviour_index; }			// Get the index of the behaviour

	void SetBehaviourName(const std::string& name) { behaviour_name = name; }
	std::string GetBehaviourName() noexcept { return behaviour_name; }
	
	virtual ComponentType TypeId() const override { return ComponentType::Behaviour; }

	int behaviour_index;
	std::string behaviour_name;
};