#include <Debug.h>
#include <Factory.h>
#include <PhysicsSystem.h>
#include <components/Body.h>
#include <Interface_System.h>
#include <typeinfo>
#include <string>
#include <iostream>
#include <components/Physics.h>
#include <components/PlayerControllable.h>
#include <input.h>

Vec2 interPt, normalAtCollision;
float interTime = 0.0f;

/*
* This function is meant to update the gravity of a game object.
* In the future, this will be changed to take in a game object struct
* once we implement that in the game state update function.
*/
/*
void gravityUpdate(int* objYVelocity)
{
	*objYVelocity = gravity * frameTime + *objYVelocity;
}
*/

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
		return Collision::Check_AABB_AABB(((Rectangular*)b1)->aabb, ((Transform*)b1)->Position, ((Rectangular*)b2)->aabb, ((Transform*)b2)->PrevPosition, dt);
	}

	else {
		return false; // Unsupported collision
	}
}

PhysicsSystem::PhysicsSystem() {

}

void PhysicsSystem::Initialize() {

}

void PhysicsSystem::Update(float time) {
	// Update velocity for player
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {

		if ((PlayerControllable*)obj->second->GetComponent(ComponentType::PlayerControllable) == nullptr)
			continue; // That object is not controlled by the player, move to the next object

		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);

		p->X_Velocity = 0.0f; // Reset velocity
		
		//ALL THE input::IsPressed() IS BROKEN SOMEHOW IN THIS FUNCTION :/

		if (input::IsPressedRepeatedly(KEY::d)) {
			p->X_Velocity += 10.0f;
			std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
		}
		if (input::IsPressedRepeatedly(KEY::a)) {
			p->X_Velocity -= 10.0f;
			std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
		}

		if (input::IsPressedRepeatedly(KEY::w) && p->Y_Velocity != 0.0f) { // Jump. Make sure vertical velocity is 0 first
			p->Y_Velocity = 10.0f;
			std::cout << "Current player velocity: x=" << p->X_Velocity << ", y=" << p->Y_Velocity << std::endl;
		}
		
		// DEBUG: Output player's position and velocity by holding F
		if (input::IsPressed(KEY::f) || input::IsPressedRepeatedly(KEY::f)) {
			std::cout << "======================================================================================" << std::endl;
			std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
			std::cout << "Current player velocity: x=" << p->X_Velocity << ", y=" << p->Y_Velocity << std::endl;
			std::cout << "Current player acceleration: y=" << p->Y_Acceleration << std::endl;
		}

		break; // There should only be one object that is player controlled for now
	}

	// Update velocity for each object
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
	//for (const std::pair<const unsigned int, Object*>& pair : gameObjFactory->objMap) {
		Physics *p = (Physics*) obj->second->GetComponent(ComponentType::Physics);

		// DEBUG: Print address to stdout
		//std::cout << t << std::endl;

		if (p == nullptr)
			continue; // No physics in that object, move along

		// No X acceleration, not needed in the game

		p->Y_Velocity = gravity * time + p->Y_Velocity; // Apply gravity

		//p->Y_Velocity += p->Y_Acceleration * time;
		p->Y_Velocity *= 0.95f; // Account for air resistance
	}

	// Recalculate collision data inside each object
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Body* b = (Body*)obj->second->GetComponent(ComponentType::Body);
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);

		if (b == nullptr || t == nullptr)
			continue; // Collision not enabled for that object, move along

		// Only recalculate if position and previous position are not the same
		if (t->Position != t->PrevPosition) {
			Vec2 pos = ((Transform*)obj->second->GetComponent(ComponentType::Transform))->Position;

			if (b->GetShape() == Shape::Rectangle) {
				((Rectangular*)b)->aabb.min = pos - Vec2(((Rectangular*)b)->width / 2, ((Rectangular*)b)->height / 2);
				((Rectangular*)b)->aabb.max = pos + Vec2(((Rectangular*)b)->width / 2, ((Rectangular*)b)->height / 2);
			}
			else if (b->GetShape() == Shape::Circle) {
				((Circular*)b)->circle.center = pos;
			}
		}
	}

	for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);
		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);

		if (p == nullptr)
			continue; // No physics in that object, move along

		// Save current position to previous position
		t->PrevPosition = t->Position;

		if (p->X_Velocity == 0.f && p->Y_Velocity == 0.f)
			continue; // No movement, so no need to calculate collision.

		bool hasCollided = false;

		for (Factory::objectIDMap::iterator anotherobj = std::next(obj); anotherobj != objectFactory->objectMap.end(); ++anotherobj) {
			if (Check_Collision((Body*)obj->second->GetComponent(ComponentType::Body), (Body*)anotherobj->second->GetComponent(ComponentType::Body), time)) {
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
				hasCollided = true;
			}
		}

		if (hasCollided) {
			// For now, set all velocity to 0
			p->X_Velocity = 0.f;
			p->Y_Velocity = 0.f;
		} else {
			t->Position.x += p->X_Velocity;
			t->Position.y += p->Y_Velocity;
		}
	}
}
