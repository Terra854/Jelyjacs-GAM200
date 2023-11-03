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
	Behaviour() : Component(), behaviour_index{ 0 }, obj{ NULL } {}

	//~Behaviour() = default;
	virtual void Initialize() override {}
	void SetIndex(const int& index);	// Set the index of the behaviour
	int& GetIndex() noexcept;			// Get the index of the behaviour

	virtual ComponentType TypeId() const override { return ComponentType::Behaviour; }

	int behaviour_index;
	std::string behaviour_name;
	Object* obj;
};