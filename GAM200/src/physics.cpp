#include <Debug.h>
#include <Factory.h>
#include <physics.h>
#include <components/Body.h>
#include <Interface_System.h>
#include <typeinfo>
#include <string>
#include <iostream>

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

Physics::Physics() {

}

void Physics::Initialize() {

}

void Physics::Update(float time) {
	// DEBUG: Make sure it's running
	// std::cout << "Physics::Update" << std::endl;

	// Update velocity for each object
	for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
	//for (const std::pair<const unsigned int, Object*>& pair : gameObjFactory->objMap) {
		Transform *t = (Transform*) obj->second->GetComponent(ComponentType::Transform);

		// DEBUG: Print address to stdout
		//std::cout << t << std::endl;

		if (t == nullptr)
			continue; // No transform in that object, move along

		t->X_Velocity += t->X_Acceleration;
		t->Y_Velocity += t->Y_Acceleration;
	}

	for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);

		// Save current position to previous position
		t->PrevPosition = t->Position;

		if (t->X_Velocity == 0.f && t->Y_Velocity == 0.f)
			continue; // No movement, so no need to calculate collision.

		bool hasCollided = false;

		for (Factory::objectIDMap::iterator anotherobj = std::next(obj); anotherobj != objectFactory->objectMap.end(); ++anotherobj) {
			if (Check_Collision((Body*)obj->second->GetComponent(ComponentType::Body), (Body*)anotherobj->second->GetComponent(ComponentType::Body))) {
				
				// DEBUG
				std::cout << "A collision has occured between ";
				switch (((Body*)obj->second->GetComponent(ComponentType::Body))->GetShape()) {
				case shape::rect:
						std::cout << "a rectangle ";
						break;
				case shape::circle:
					std::cout << "a circle ";
					break;
				case shape::line:
					std::cout << "a line ";
					break;
				}
				std::cout << "and ";
				switch (((Body*)anotherobj->second->GetComponent(ComponentType::Body))->GetShape()) {
				case shape::rect:
					std::cout << "a rectangle.";
					break;
				case shape::circle:
					std::cout << "a circle.";
					break;
				case shape::line:
					std::cout << "a line.";
					break;
				}

				hasCollided = true;
			}
		}

		if (hasCollided) {
			// For now, set all velocity to 0
			t->X_Velocity = 0.f;
			t->Y_Velocity = 0.f;
		} else {
			t->Position.x += t->X_Velocity;
			t->Position.y += t->Y_Velocity;
		}
	}
}
