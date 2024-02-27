#pragma once
/* !
@file Behaviour.h
@author Luke Goh
@date	27/11/2023

This file contains the Behaviour Component that is needed for Logic System
*//*__________________________________________________________________________*/

#include <Object.h>
#include <iostream>

// Behaviour Component
// Class Behaviour, derived from the Component class.
// This class represents a generic behavior that can be attached to game objects.
class Behaviour : public Component
{
public:
    // Default constructor for the Behaviour class.
    // Initializes the behaviour with a default index and name.
    Behaviour() : Component(), behaviour_index{ 999 }, behaviour_name{ "NULL" } {}

    // Parameterized constructor for the Behaviour class.
    // @param index: An integer representing the index of the behavior.
    // @param name: A string representing the name of the behavior.
    Behaviour(float index, std::string name) : Component(), behaviour_index{ index }, behaviour_name{ name } {}

    // Default destructor for the Behaviour class.
    // It is marked as default to use the compiler-generated destructor.
    ~Behaviour() = default;

    // Initialize method, typically used to set up the behavior.
    // This method is virtual and can be overridden in derived classes.
    virtual void Initialize() override {}

    // Setter for the behaviour index.
    // @param index: The new index to set for this behavior.
    void SetBehaviourIndex(float index) {
        behaviour_index = index;
    }

    // Getter for the behaviour index.
    // @return: Returns the index of this behavior.
    float GetBehaviourIndex() noexcept { return behaviour_index; }

    // Setter for the behaviour name.
    // @param name: The new name to set for this behavior.
    void SetBehaviourName(std::string name) {
        behaviour_name = name;
        std::cout << "Behaviour Name Set : " << behaviour_name << " for " << GetOwner()->GetName() << " with ID " << GetOwner()->GetId() << std::endl;
    }

    // Getter for the behaviour name.
    // @return: Returns the name of this behavior.
    std::string GetBehaviourName() noexcept { return behaviour_name; }

    // TypeId method to return the type of this component (Behaviour).
    // @return: Returns the type ID of this component.
    virtual ComponentType TypeId() const override { return ComponentType::Behaviour; }

private:
    float behaviour_index;        // An integer to store the index of the behavior.
    std::string behaviour_name; // A string to store the name of the behavior.
};
