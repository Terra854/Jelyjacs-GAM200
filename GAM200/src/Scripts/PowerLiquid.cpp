/* !
@file P_liquid.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the p_liquid for the human to move
*//*__________________________________________________________________________*/
#include "Scripts/PowerLiquid.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>
#include <Core_Engine.h>
#include <GameLogic.h>

bool collected = false;
namespace PL_Script {
	// Constructor for the P_liquid class.
	// @param name: The name of the p_liquid.
	P_liquid::P_liquid(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
	}
	/*********************************************************************/
	// Start method called when the p_liquid script is ready.
	// @param obj: The object to which this script is attached.
	/*********************************************************************/
	void P_liquid::Start(Object* obj) {
		std::cout << "P_liquid Script Ready : " << obj->GetName() << std::endl;
		collected = false;
	}

	/*********************************************************************/
	// Update method called every frame to update the p_liquid's logic.
	// @param obj: The object to which this script is attached.
	/*********************************************************************/
	void P_liquid::Update(Object* obj) {
		if (obj == nullptr || GameLogic::playerObj == nullptr ) {
			//std::cout << "NIL OBJ : P_liquid" << std::endl;
			return;
		}

		// Get the Body components of Power Liquid & Player.
		Rectangular* player_b = static_cast<Rectangular*>(GameLogic::playerObj->GetComponent(ComponentType::Body));
		Rectangular* p_liquid_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));


		if (player_b == nullptr || p_liquid_b == nullptr) {
			std::cout << "NIL COMPONENT : P_liquid" << std::endl;
			return;
		}
		if (player_b->collision_flag & COLLISION_RIGHT)
		if (player_b->right_collision != nullptr && static_cast<Rectangular*>(player_b->right_collision->GetComponent(ComponentType::Body)) == p_liquid_b) {
				std::cout << "Right Collision : " << player_b->right_collision->GetName() << std::endl;
				collected = true;
		}

		if (player_b->collision_flag & COLLISION_LEFT)
			if (player_b->left_collision != nullptr && static_cast<Rectangular*>(player_b->left_collision->GetComponent(ComponentType::Body)) == p_liquid_b) {
				std::cout << "Left Collision : " << player_b->left_collision->GetName() << std::endl;
				collected = true;
		}

		if (player_b->collision_flag & COLLISION_TOP)
			if (player_b->top_collision != nullptr && static_cast<Rectangular*>(player_b->top_collision->GetComponent(ComponentType::Body)) == p_liquid_b) {
				std::cout << "Top Collision : " << player_b->top_collision->GetName() << std::endl;
				collected = true;
		}

		if (player_b->collision_flag & COLLISION_BOTTOM)
			if (player_b->bottom_collision != nullptr && static_cast<Rectangular*>(player_b->bottom_collision->GetComponent(ComponentType::Body)) == p_liquid_b) {
				std::cout << "Bottom Collision : " << player_b->bottom_collision->GetName() << std::endl;
				collected = true;
		}
		
		// Check if the player collides with the power liquid.
		if (collected == true){
			audio->playSfx("collect_power_liquid_notification");
			audio->playSfx("collect_power_liquid");

			// Make sure to remove any references to this object otherwise
			// it will result in dangling pointers and eventually a crash
			if (player_b->right_collision == obj) {
				player_b->right_collision = nullptr;
			}
			if (player_b->left_collision == obj) {
				player_b->left_collision = nullptr;
			}
			if (player_b->top_collision == obj) {
				player_b->top_collision = nullptr;
			}
			if (player_b->bottom_collision == obj) {
				player_b->bottom_collision = nullptr;
			}

			objectFactory->destroyObject(obj);
			CatPower++;
			collected = false;
			return;
		}

	}
	/*********************************************************************/
	// Shutdown method called when the p_liquid script is being shut down.
	// @param obj: The object to which this script is attached.
	/*********************************************************************/
	void P_liquid::Shutdown(Object* obj) {
		std::cout << "P_liquid Script Shutdown : " << obj->GetName() << std::endl;
	}

	// Creating an instance of P_liquid.
	P_liquid p_liquid("Power_Liquid");
} // namespace PL_Script

