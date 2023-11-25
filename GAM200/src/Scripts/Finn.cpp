#include "Scripts/Finn.h"
#include <iostream>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <Audio.h>

Finn::Finn(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}

void Finn::Start(Object* obj) {
	std::cout << "Finn Script Ready : " << obj->GetName() << std::endl;
}

void Finn::Update(Object* obj) {
	if (obj == nullptr) {
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}

	if (GameLogic::playerObj->GetName() == "Finn")
	{
		Physics* player_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
		Animation* player_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
		if (player_physics == nullptr || player_animation == nullptr) {
			//std::cout << "NIL COMPONENT : Player" << std::endl;
			return;
		};
		player_physics->Velocity.x = 0.0f;

		if (player_animation->face_right) {
			player_animation->current_type = AnimationType::Idle;
		}
		else {
			player_animation->current_type = AnimationType::Idle_left;
		}

		bool moving = false;
		if (input::IsPressed(KEY::w)) {
			MovementKey msg(up);
			engine->Broadcast(&msg);
			//if (static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body))->collision_flag & COLLISION_BOTTOM) {
			if (player_physics->Velocity.y == 0.0f) {
				//player_physics->Velocity.y = 1000.0f;
				player_physics->Force = 85000.0f;
				//audio->playJump();
				std::cout << "PlayJump " << player_physics->GetOwner()->GetName() << std::endl;
				audio->playSfx(AudioType::Finn_Jumping);
				
			}
		}
		if (input::IsPressedRepeatedly(KEY::a)) {
			MovementKey msg(left);
			engine->Broadcast(&msg);
			player_physics->Velocity.x -= 500.0f;
			moving = true;
			player_animation->face_right = false;
			player_animation->current_type = AnimationType::Run_left;
		}
		if (input::IsPressedRepeatedly(KEY::d)) {
			MovementKey msg(right);
			engine->Broadcast(&msg);
			player_physics->Velocity.x += 500.0f;
			moving = true;
			player_animation->face_right = true;
			player_animation->current_type = AnimationType::Run;
		}

		if (player_physics->Velocity.y != 0.0f) {
			player_animation->jump_fixed = true;

			if (player_animation->face_right)player_animation->current_type = AnimationType::Jump;
			else player_animation->current_type = AnimationType::Jump_left;
		}
		else player_animation->jump_fixed = false;



		// Audio for Character Movement
		if ((player_physics->Velocity.y == 0.f) && moving) {
			audio->startWalking();
		}
		else {
			audio->stopWalking();
			moving = false;
		}
	}
}

void Finn::Shutdown(Object* obj) {
	std::cout << "Finn Script Shutdown : " << obj->GetName() << std::endl;
}

Finn finn("Finn");