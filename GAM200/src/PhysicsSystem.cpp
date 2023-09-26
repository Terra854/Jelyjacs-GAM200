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

//Vec2 interPt, normalAtCollision;
//float interTime = 0.0f;

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
			int flag = Collision::Check_Rect_Rect((Rectangular*)b1, (Rectangular*)b2);
			return true;
		}
	}

	else {
		return false; // Unsupported collision
	}
}
*/
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

void Response_Collision(int flag, Transform* t1, Body* b1, Physics* p1, Transform* t2, Body* b2) {
	// 2 Rectangles
	if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Rectangular)) {
		if (flag & COLLISION_LEFT) {
			t1->Position.x = t1->PrevPosition.x;
		}
		if (flag & COLLISION_RIGHT) {
			t1->Position.x = t1->PrevPosition.x;
		}
		if (flag & COLLISION_TOP) {
			p1->Velocity.y = 0.0f;
			t1->Position.y = t1->PrevPosition.y;
		}
		if (flag & COLLISION_BOTTOM) {
			p1->Velocity.y = 0.0f;
			t1->Position.y = t1->PrevPosition.y;
		}
	}
}

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

		p->Velocity.x = 0.0f; // Reset velocity

		// Move right
		if (input::IsPressedRepeatedly(KEY::d)) {
			p->Velocity.x += 15000.0f * time;
			std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
		}

		// Move left
		if (input::IsPressedRepeatedly(KEY::a)) {
			p->Velocity.x -= 15000.0f * time;
			std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
		}

		// Jump. Make sure vertical velocity is 0 first
		if (input::IsPressedRepeatedly(KEY::w) && p->Velocity.y == 0.0f) {
			p->Velocity.y = 2500.0f;
		}

		break; // There should only be one object that is player controlled for now
	}

	// Update velocity for each object
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);

		if (p == nullptr)
			continue; // No physics in that object, move along

		// DEBUG: Don't apply gravity to player for now
		//if ((PlayerControllable*)obj->second->GetComponent(ComponentType::PlayerControllable) != nullptr)
			//continue;

		// No X acceleration, not needed in the game

		// Apply gravity
		p->Y_Acceleration = gravity * time;
		p->Velocity.y += p->Y_Acceleration;

		p->Velocity.y *= 0.95f; // Account for air resistance
	}

	for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);
		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);
		Body* b = (Body*)obj->second->GetComponent(ComponentType::Body);

		if (p == nullptr || b == nullptr)
			continue; // No physics or body in that object, move along

		// Save current position to previous position
		if (Vec2Distance(t->PrevPosition, t->Position) >= 1.0f) {
			t->PrevPosition = t->Position;
		}

		if (p->Velocity.x == 0.f && p->Velocity.y == 0.f)
			continue; // No movement, so no need to calculate collision.

		// Calculate new position
		t->Position += p->Velocity * time;

		RecalculateBody(t, b);

		for (Factory::objectIDMap::iterator anotherobj = objectFactory->objectMap.begin(); anotherobj != objectFactory->objectMap.end(); ++anotherobj) {

			if (obj == anotherobj)
				continue; // Can't collide with yourself

			Body* b2 = (Body*)anotherobj->second->GetComponent(ComponentType::Body);

			if (b2 == nullptr)
				continue; // No body in the other object, no way it's collidable

			if (int flag = Check_Collision(b, b2, time)) {

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

				/*
				// DEBUG: Print out collision flags
				std::cout << "FLAG: " << flag <<
					" LEFT: " << ((flag & COLLISION_LEFT) ? "YES" : "NO") <<
					" RIGHT: " << ((flag & COLLISION_RIGHT) ? "YES" : "NO") <<
					" TOP: " << ((flag & COLLISION_TOP) ? "YES" : "NO") <<
					" BOTTOM: " << ((flag & COLLISION_BOTTOM) ? "YES" : "NO") << std::endl;
				*/

				Response_Collision(flag, t, b, p, (Transform*)anotherobj->second->GetComponent(ComponentType::Transform), b2);
			}
		}
	}
}
