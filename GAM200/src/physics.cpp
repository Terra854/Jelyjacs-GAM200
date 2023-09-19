#include <Factory.h>
#include <physics.h>
#include <components/Body.h>
#include <Interface_System.h>
#include <typeinfo>
#include <string>
#include <iostream>

Vec2 interPt, normalAtCollision;
float interTime = 0.0f;

extern GameObjectFactory* factory;

/*
* This function is meant to update the gravity of a game object.
* In the future, this will be changed to take in a game object struct
* once we implement that in the game state update function.
*/
/*
void gravityUpdate(int* gameobjectYVelocity)
{
	*gameobjectYVelocity = gravity * frameTime + *gameobjectYVelocity;
}
*/

bool Check_Collision(Body* b1, Body* b2) {

	if (typeid(*b1) == typeid(Circlular) && typeid(*b2) == typeid(lines)) {
		return Collision::Check_Circle_Line(((Circlular*)b1)->cirlce, ((Transform*)b1)->PrevPosition, ((lines*)b2)->line, interPt, normalAtCollision, interTime);
	}
	if (typeid(*b1) == typeid(lines) && typeid(*b2) == typeid(Circlular)) {
		return Collision::Check_Circle_Line(((Circlular*)b2)->cirlce, ((Transform*)b2)->PrevPosition, ((lines*)b1)->line, interPt, normalAtCollision, interTime);
	}

	else if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(lines)) {
		return Collision::Check_AABB_Line(((Rectangular*)b1)->aabb, ((Transform*)b1)->PrevPosition, ((lines*)b2)->line, interPt, normalAtCollision, interTime);
	}
	else if (typeid(*b1) == typeid(lines) && typeid(*b2) == typeid(Rectangular)) {
		return Collision::Check_AABB_Line(((Rectangular*)b2)->aabb, ((Transform*)b2)->PrevPosition, ((lines*)b1)->line, interPt, normalAtCollision, interTime);
	}

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
	std::cout << "Physics::Update" << std::endl;

	// Update velocity for each object
	for (auto gameObject = factory->gameObjectMap.begin(); gameObject != factory->gameObjectMap.end(); ++gameObject) {
	//for (const std::pair<const unsigned int, GOC*>& pair : factory->gameObjectMap) {
		Transform *t = (Transform*) gameObject->second->GetComponent(ComponentTypeId::CT_Transform);

		// DEBUG: Print address to stdout
		std::cout << t << std::endl;

		if (t == nullptr)
			continue; // No transform in that object, move along

		t->X_Velocity += t->X_Acceleration;
		t->Y_Velocity += t->Y_Acceleration;
	}

	for (GameObjectFactory::gameObjIDMap::iterator gameObject = factory->gameObjectMap.begin(); gameObject != factory->gameObjectMap.end(); ++gameObject) {
		Transform* t = (Transform*)gameObject->second->GetComponent(ComponentTypeId::CT_Transform);

		// Save current position to previous position
		t->PrevPosition = t->Position;

		if (t->X_Velocity == 0.f && t->Y_Velocity == 0.f)
			continue; // No movement, so no need to calculate collision.

		bool hasCollided = false;

		for (GameObjectFactory::gameObjIDMap::iterator anotherGameObject = std::next(gameObject); anotherGameObject != factory->gameObjectMap.end(); ++anotherGameObject) {
			if (Check_Collision((Body*)gameObject->second->GetComponent(ComponentTypeId::CT_Body), (Body*)anotherGameObject->second->GetComponent(ComponentTypeId::CT_Body))) {
				
				// DEBUG
				std::cout << "A collision has occured between ";
				switch (((Body*)gameObject->second->GetComponent(ComponentTypeId::CT_Body))->GetShape()) {
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
				switch (((Body*)anotherGameObject->second->GetComponent(ComponentTypeId::CT_Body))->GetShape()) {
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

std::string GetName() { return "Physics"; };
