#include "Scripts/Player.h"
#include <PhysicsSystem.h>
#include <components/Physics.h>
#include <components/Animation.h>
#include <components/Transform.h>
#include <components/Body.h>
#include <components/PlayerControllable.h>
#include <components/Event.h>
#include <Audio.h>

Player::Player(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}

void Player::Start(Object* obj) {
	std::cout << "Player Script Ready : "<< obj->GetName() << std::endl;
}

void Player::Update(Object* obj) {
	if (input::IsPressed(KEY::down)) {
		std::cout << "Down is working for player" << std::endl;
	}
	// For some reason, the player is not changing position
	/*
	if ((obj == nullptr) || (obj->GetComponent(ComponentType::Physics) == nullptr)) return;
	Physics* player_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	Animation* player_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	player_physics->Velocity.x = 0.0f;

	if (player_animation->face_right){
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
			player_physics->Force = 60000.0f + gravity;
			audio->playJump();
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
	*/
	
}

void Player::Shutdown(Object* obj){

}

Player player("Player");

