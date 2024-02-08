/* !
@file	Piston.h
@author Luke Goh
@date	27/11/2023

This file contains the script for Piston
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Piston, derived from LogicScript.
// This class is intended to encapsulate the behavior and logic of a piston mechanism in a game.
// It provides a framework for managing the piston's initialization, regular updates, and shutdown process.
class Piston : public LogicScript
{
public:
    // Constructor for the Piston class.
    // @param name: A string representing the name of the Piston instance.
    // This constructor initializes the piston, setting up any necessary states or configurations.
    Piston(std::string name);

    // Default destructor for the Piston class.
    // Marked as default to use the compiler-generated destructor.
    ~Piston() = default;

    // Start method, called when the Piston script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // Typically used for initial setup and configuration of the piston.
    void Start(Object* obj);

    // Update method, called on every frame to update the piston's state.
    // @param obj: A pointer to the Object that this script is attached to.
    // Contains the logic for the piston's movement, interactions, and other behaviors.
    void Update(Object* obj);

    // Shutdown method, called when the Piston script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // Used for cleanup tasks or to reset the state of the piston.
    void Shutdown(Object* obj);
};

/*
class StateMachine 
{
public:
    StateMachine();
	~StateMachine();

	void AddState(State* state);
	void AddTransition(Transition* transition);
	void SetInitialState(State* state);
	void SetCurrentState(State* state);
	void Update();
	void Shutdown();
private:
	std::vector<State*> states;
	std::vector<Transition*> transitions;
	State* initial_state;
	State* current_state;
};

class State
{
public:
    State();
	~State();
private:
	std::string state_name;
	StateMachine* state_machine;
	std::vector<Transition*> transitions;
};
};

class Transition
{
public:
	Transition();
	~Transition();

	void OnEnter();
	void OnUpdate();
	void OnExit();
private:
    std::string transition_name;
	State* from_state;
	State* to_state;
};
*/