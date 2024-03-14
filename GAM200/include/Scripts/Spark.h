/* !
@file	Spark.h
@author Luke Goh
@date	27/11/2023

This file contains the script for Spark
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>
#include <Vec2.h>

enum TeleportingState {
    None = 0,
    Disappearing,
    Appearing
};

// Class Spark, derived from LogicScript.
// This class is designed to manage the behavior and state of a character or entity named Spark in a game.
// It includes functionalities for initializing, updating, and shutting down the Spark entity.
class Spark : public LogicScript
{
public:
    // Constructor for the Spark class.
    // @param name: A string representing the name of the Spark instance.
    // Initializes the Spark entity and potentially sets up initial states or configurations.
    Spark(std::string name);

    // Default destructor for the Spark class.
    // Marked as default to use the compiler-generated destructor.
    ~Spark() = default;

    // Start method, called when the Spark script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // Typically used for initial setup and configuration for Spark.
    void Start(Object* obj);

    // Update method, called on every frame to update Spark's state.
    // @param obj: A pointer to the Object that this script is attached to.
    // Contains the logic for Spark's behavior and interactions during the game.
    void Update(Object* obj);

    // Shutdown method, called when the Spark script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // Used for cleanup tasks or to reset the state of Spark.
    void Shutdown(Object* obj);

    // A boolean member variable to track if Spark is currently connected to another character/entity named Finn.
    // This might be used to handle interactions or dependencies between Spark and Finn.
    static bool Connected_to_Finn;

    // A static boolean member variable to track if Spark has just detached from Finn.
    // This could be used to manage the transition or effects of detachment in the game logic.
    static bool Just_detached;

    static bool teleporting;
    static bool UsedPower;
    static TeleportingState teleporting_state;

    static Vec2 next_position;
};
