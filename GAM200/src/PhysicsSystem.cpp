/* !
@file	PhysicsSystem.cpp
@author	Tan Yee Ann
@date	28/9/2023

This file contains the definitions of the functions that are part of the Physics system
*//*__________________________________________________________________________*/
#include <Debug.h>
#include <Factory.h>
#include <PhysicsSystem.h>
#include <components/Body.h>
#include <Interface_System.h>
#include <Core_Engine.h>
#include <typeinfo>
#include <string>
#include <iostream>
#include <components/Physics.h>
#include <components/PlayerControllable.h>
#include <input.h>
#include <Audio.h> // Direct call the audio functions cause messaging system is not ready

//Vec2 interPt, normalAtCollision;
//float interTime = 0.0f;

int collision_flag;

// A workaround to prevent sticking onto the top of the walls
float top_collision_cooldown = 0.f;



// Old version of Check_Collision, might be deleted at some point

/*
bool Check_Collision(Body* b1, Body* b2, float dt) {

	// Circle and Line
	if (typeid(*b1) == typeid(Circular) && typeid(*b2) == typeid(Lines)) {
		return Collision::Check_Circle_Line(((Circular*)b1)->circle, ((Transform*)b1)->Position, ((Lines*)b2)->line, interPt, normalAtCollision, interTime);
	}
	if (typeid(*b1) == typeid(Lines) && typeid(*b2) == typeid(Circular)) {
		return Collision::Check_Circle_Line(((Circular*)b2)->circle, ((Transform*)b2)->Position, ((Lines*)b1)->line, interPt, normalAtCollision, interTime);
	}

	// Rectangle and Line
	else if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Lines)) {
		return Collision::Check_AABB_Line(((Rectangular*)b1)->aabb, ((Transform*)b1)->Position, ((Lines*)b2)->line, interPt, normalAtCollision, interTime);
	}
	else if (typeid(*b1) == typeid(Lines) && typeid(*b2) == typeid(Rectangular)) {
		return Collision::Check_AABB_Line(((Rectangular*)b2)->aabb, ((Transform*)b2)->Position, ((Lines*)b1)->line, interPt, normalAtCollision, interTime);
	}

	// 2 Rectangles
	else if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Rectangular)) {
		if (Collision::Check_AABB_AABB(((Rectangular*)b1)->aabb, ((Transform*)b1)->Position, ((Rectangular*)b2)->aabb, ((Transform*)b2)->PrevPosition, interPt, normalAtCollision, interTime)){
			int collision_flag = Collision::Check_Rect_Rect((Rectangular*)b1, (Rectangular*)b2);
			return true;
		}
	}

	else {
		return false; // Unsupported collision
	}
}
*/

// Check if both bodies are rectangular
// If they are, use Check AABB with AABB function and return collision flag.
// If return false means no collision
int Check_Collision(Body* b1, Body* b2, float dt) {
	// 2 Rectangles
	if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Rectangular)) {
		if (Collision::Check_AABB_AABB(((Rectangular*)b1)->aabb, ((Transform*)b1)->Position, ((Rectangular*)b2)->aabb, ((Transform*)b2)->PrevPosition, dt))
			return Collision::Check_Rect_Rect((Rectangular*)b1, (Rectangular*)b2);
	}
	else {
		return false; // Unsupported collision
	}
	return false;
}

// If collide on the left or right, unable to move
// If collide from the top ...
// If collide from the bottom ...
// Recalculate collision data
void RecalculateBody(Transform* t, Body* b) {
	// Recalculate AABB for rectangles
	if (b->GetShape() == Shape::Rectangle) {
		((Rectangular*)b)->aabb.min = t->Position - Vec2(((Rectangular*)b)->width / 2, ((Rectangular*)b)->height / 2);
		((Rectangular*)b)->aabb.max = t->Position + Vec2(((Rectangular*)b)->width / 2, ((Rectangular*)b)->height / 2);
	}

	// Recalculate Center for rectangles
	else if (b->GetShape() == Shape::Circle) {
		((Circular*)b)->circle.center = t->Position;
	}
}

// Objects responding to collision
void Response_Collision(Transform* t1, Body* b1, Physics* p1, Transform* t2, Body* b2) {
	(void)t2;
	// 2 Rectangles
	if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Rectangular)) {

		int collision_flag = ((Rectangular*)b1)->collision_flag;

		if (collision_flag & COLLISION_LEFT) {
			t1->Position.x = t1->PrevPosition.x;
		}
		if (collision_flag & COLLISION_RIGHT) {
			t1->Position.x = t1->PrevPosition.x;
		}
		if (collision_flag & COLLISION_TOP) {
			top_collision_cooldown = 0.1f;
			p1->Velocity.y = 0.0f;
			t1->Position.y = ((Rectangular*)b2)->aabb.min.y - (((Rectangular*)b1)->height / 2);
		}
		if (collision_flag & COLLISION_BOTTOM) {
			p1->Velocity.y = 0.0f;
			t1->Position.y = ((Rectangular*)b2)->aabb.max.y + (((Rectangular*)b1)->height / 2);
		}

		if (collision_flag)
			RecalculateBody(t1, b1);
	}
}

PhysicsSystem::PhysicsSystem() {

}

void PhysicsSystem::Initialize() {

}

void PhysicsSystem::Update() {

	float dt = engine->GetDt();
	// If there is a sudden lag spike, the physics will act weird
	// In that case, do not update for this cycle
	if (dt > 0.05f) {
		return;
	}

	top_collision_cooldown = (top_collision_cooldown > 0.0f) ? top_collision_cooldown -= dt : 0.0f;

	// Update velocity for player
	// Check all the objects, if it have the player controllable component, change its physics
	/*
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {

		if ((PlayerControllable*)obj->second->GetComponent(ComponentType::PlayerControllable) == nullptr)
			continue; // That object is not controlled by the player, move to the next object

		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);

		p->Velocity.x = 0.0f; // Reset velocity

		// Move right
		if (input::IsPressedRepeatedly(KEY::d)) {
			p->Velocity.x += 17500.0f * dt;
			std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
			if (p->Velocity.y == 0.f)
				audio->startWalking();
		}

		// Move left
		if (input::IsPressedRepeatedly(KEY::a)) {
			p->Velocity.x -= 17500.0f * dt;
			std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
			if (p->Velocity.y == 0.f)
				audio->startWalking();
		}

		// Stop the walking audio if the player is neither walking nor on solid ground
		if ((!(input::IsPressedRepeatedly(KEY::d)) && !(input::IsPressedRepeatedly(KEY::a))) || p->Velocity.y != 0.f){
			audio->stopWalking();
		}

		// Jump. Make sure vertical velocity is 0 first
		if (input::IsPressedRepeatedly(KEY::w) && p->Velocity.y == 0.0f && top_collision_cooldown == 0.0f) {
			p->Velocity.y = 2500.0f;
			audio->playJump();
		}
		
		break; // There should only be one object that is player controlled for now
	}
	*/

	// Update velocity for each object
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);

		if (p == nullptr)
			continue; // No physics in that object, move along

		// No X acceleration, not needed in the game

		// Apply gravity
		p->Y_Acceleration = gravity;
		p->Velocity.y += (float) (p->Y_Acceleration - 0.75 * p->Velocity.y) * dt; // Account for air resistance
	}

	// Loop through each object to see if it's colliding with something
	for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);
		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);
		Body* b = (Body*)obj->second->GetComponent(ComponentType::Body);

		if (p == nullptr || b == nullptr)
			continue; // No physics or body in this object, move to next object

		// Save current position to previous position
		t->PrevPosition = t->Position;

		if (p->Velocity.x == 0.f && p->Velocity.y == 0.f)
			continue; // No movement, so no need to calculate collision.

		// Reset collision flags
		if (b->GetShape() == Shape::Rectangle)
			((Rectangular*)b)->collision_flag = 0;

		// Calculate new position
		t->Position += p->Velocity * dt;

		RecalculateBody(t, b);

		for (Factory::objectIDMap::iterator anotherobj = objectFactory->objectMap.begin(); anotherobj != objectFactory->objectMap.end(); ++anotherobj) {

			if (obj == anotherobj)
				continue; // Can't collide with yourself

			Body* b2 = (Body*)anotherobj->second->GetComponent(ComponentType::Body);

			if (b2 == nullptr)
				continue; // No body in the other object, no way it's collidable

			if (collision_flag = Check_Collision(b, b2, dt)) {

				/*
				// DEBUG
				std::cout << "A collision has occured between ";
				switch (((Body*)obj->second->GetComponent(ComponentType::Body))->GetShape()) {
				case Shape::Rectangle:
					std::cout << "a rectangle ";
					break;
				case Shape::Circle:
					std::cout << "a circle ";
					break;
				case Shape::Line:
					std::cout << "a line ";
					break;
				}
				std::cout << "and ";
				switch (((Body*)anotherobj->second->GetComponent(ComponentType::Body))->GetShape()) {
				case Shape::Rectangle:
					std::cout << "a rectangle.";
					break;
				case Shape::Circle:
					std::cout << "a circle.";
					break;
				case Shape::Line:
					std::cout << "a line.";
					break;
				}
				std::cout << std::endl;
				*/
				Response_Collision(t, b, p, (Transform*)anotherobj->second->GetComponent(ComponentType::Transform), b2);
			}
			else {
				
			}
		}
	}
}

void PhysicsSystem::MessageRelay(Message_Handler* msg) {
	if (msg->GetMessage() == MessageID::Collide) {

	}
}
