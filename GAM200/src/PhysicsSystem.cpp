#include <Debug.h>
#include <Factory.h>
#include <PhysicsSystem.h>
#include <components/Body.h>
#include <Interface_System.h>
#include <typeinfo>
#include <string>
#include <iostream>
#include <components/Physics.h>

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

bool Check_Collision(Body* b1, Body* b2) {

	// Circle and Line
	if (typeid(*b1) == typeid(Circular) && typeid(*b2) == typeid(Lines)) {
		return Collision::Check_Circle_Line(((Circular*)b1)->circle, ((Transform*)b1)->PrevPosition, ((Lines*)b2)->line, interPt, normalAtCollision, interTime);
	}
	if (typeid(*b1) == typeid(Lines) && typeid(*b2) == typeid(Circular)) {
		return Collision::Check_Circle_Line(((Circular*)b2)->circle, ((Transform*)b2)->PrevPosition, ((Lines*)b1)->line, interPt, normalAtCollision, interTime);
	}

	// Rectangle and Line
	else if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Lines)) {
		return Collision::Check_AABB_Line(((Rectangular*)b1)->aabb, ((Transform*)b1)->PrevPosition, ((Lines*)b2)->line, interPt, normalAtCollision, interTime);
	}
	else if (typeid(*b1) == typeid(Lines) && typeid(*b2) == typeid(Rectangular)) {
		return Collision::Check_AABB_Line(((Rectangular*)b2)->aabb, ((Transform*)b2)->PrevPosition, ((Lines*)b1)->line, interPt, normalAtCollision, interTime);
	}

	// 2 Rectangles
	else if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Rectangular)) {
		return Collision::Check_AABB_AABB(((Rectangular*)b1)->aabb, ((Transform*)b1)->PrevPosition, ((Rectangular*)b2)->aabb, ((Transform*)b2)->PrevPosition);
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
	// DEBUG: Make sure it's running
	// std::cout << "Physics::Update" << std::endl;

	// Update velocity for each object
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
	//for (const std::pair<const unsigned int, Object*>& pair : gameObjFactory->objMap) {
		Physics *p = (Physics*) obj->second->GetComponent(ComponentType::Physics);

		// DEBUG: Print address to stdout
		//std::cout << t << std::endl;

		if (p == nullptr)
			continue; // No physics in that object, move along

		p->X_Velocity += p->X_Acceleration;
		p->Y_Velocity += p->Y_Acceleration;
	}

	for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);
		Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);

		// Save current position to previous position
		t->PrevPosition = t->Position;

		if (p->X_Velocity == 0.f && p->Y_Velocity == 0.f)
			continue; // No movement, so no need to calculate collision.

		bool hasCollided = false;

		for (Factory::objectIDMap::iterator anotherobj = std::next(obj); anotherobj != objectFactory->objectMap.end(); ++anotherobj) {
			if (Check_Collision((Body*)obj->second->GetComponent(ComponentType::Body), (Body*)anotherobj->second->GetComponent(ComponentType::Body))) {
				
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
