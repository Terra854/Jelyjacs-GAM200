/* !
@file	PhysicsSystem.cpp
@author	Tan Yee Ann (t.yeeann@digipen.edu)
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

PhysicsSystem* physics;

int collision_flag;

// A workaround to prevent sticking onto the top of the walls
float top_collision_cooldown = 0.f;

// For fixed physics
float accumulator = 0.f;

// Check if both bodies are rectangular
// If they are, use Check AABB with AABB function and return collision flag.
// If return false means no collision

/******************************************************************************
	RecalculateBody
	- Check if two objects are colliding with each other

	@param b1 = The object's body component
	@param b2 = The other object's body component
	@param dt = Delta time (preferably fixed_dt)

	@return Whether the objects are collising or not
*******************************************************************************/
bool Check_Collision(Body* b1, Body* b2, float dt) {
	// 2 Rectangles
	if (typeid(*b1) == typeid(Rectangular) && typeid(*b2) == typeid(Rectangular)) {
		if (Collision::Check_AABB_AABB(((Rectangular*)b1)->aabb, ((Transform*)b1)->Position, ((Rectangular*)b2)->aabb, ((Transform*)b2)->PrevPosition, dt)) {
			Collision::Check_Rect_Rect((Rectangular*)b1, (Rectangular*)b2);
			return true;
		}
	}
	else {
		return false; // Unsupported collision
	}
	return false;
}

// If collide on the left or right, unable to move
// If collide from the top ...
// If collide from the bottom ...

/******************************************************************************
	RecalculateBody
	- Recalculate collision data

	@param t = The object's transform component
	@param b = The object's body component
*******************************************************************************/
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

/******************************************************************************
	Response_Collision
	- Objects responding to collision
*******************************************************************************/
void Response_Collision(Transform* t1, Body* b1, Physics* p1) {
	// 2 Rectangles
	if (typeid(*b1) == typeid(Rectangular)) {

		// Make sure the top and bottom collision objects are not the same, otherwise the response will exhibit undefined behaviour
		if (!(((Rectangular*)b1)->top_collision == ((Rectangular*)b1)->bottom_collision)) {
			if (((Rectangular*)b1)->collision_flag & COLLISION_TOP) {

				top_collision_cooldown = 0.1f;
				p1->Velocity.y = 0.0f;
				t1->Position.y = ((Rectangular*)((Rectangular*)b1)->top_collision->GetComponent(ComponentType::Body))->aabb.min.y - (((Rectangular*)b1)->height / 2);

				// For objects on moving platforms
				if (((Physics*)((Rectangular*)b1)->top_collision->GetComponent(ComponentType::Physics)) != nullptr) {
					if (((Physics*)((Rectangular*)b1)->top_collision->GetComponent(ComponentType::Physics))->Velocity.y < 0.f) { // Check to see if the platform is going down
						p1->Velocity.y += ((Physics*)((Rectangular*)b1)->top_collision->GetComponent(ComponentType::Physics))->Velocity.y + (gravity * engine->Get_Fixed_DT()); // Inherit the platform's velocity and add gravity
						t1->Position.y += p1->Velocity.y * engine->Get_Fixed_DT();
					}
				}
			}
			if (((Rectangular*)b1)->collision_flag & COLLISION_BOTTOM) {

				p1->Velocity.y = 0.0f;
				t1->Position.y = ((Rectangular*)((Rectangular*)b1)->bottom_collision->GetComponent(ComponentType::Body))->aabb.max.y + (((Rectangular*)b1)->height / 2);

				// For objects on moving platforms

				Physics* moving_platform = (Physics*)((Rectangular*)b1)->bottom_collision->GetComponent(ComponentType::Physics);

				if (moving_platform != nullptr && moving_platform->Velocity.x && !(((Rectangular*)b1)->collision_flag & COLLISION_LEFT) && !(((Rectangular*)b1)->collision_flag & COLLISION_RIGHT)) {
					t1->Position.x += ((Physics*)((Rectangular*)b1)->bottom_collision->GetComponent(ComponentType::Physics))->Velocity.x * engine->Get_Fixed_DT();
					t1->Position.y += ((Physics*)((Rectangular*)b1)->bottom_collision->GetComponent(ComponentType::Physics))->Velocity.y * engine->Get_Fixed_DT();
				}
			}
		}

		// Make sure the left and right collision objects are not the same, otherwise the response will exhibit undefined behaviour
		if (!(((Rectangular*)b1)->left_collision == ((Rectangular*)b1)->right_collision)) {
			if (((Rectangular*)b1)->collision_flag & COLLISION_LEFT) {
				Object* leftObj = ((Rectangular*)b1)->left_collision;

				// For objects that are pushable
				if (p1->AbleToPushObjects && ((Rectangular*)leftObj->GetComponent(ComponentType::Body))->pushable) {
					// Pulling the object
					if (input::IsPressedRepeatedly(KEY::k) && input::IsPressedRepeatedly(KEY::a)) {
						// Make sure there's no objects on the right
						if (((Rectangular*)b1)->right_collision == nullptr) {
							t1->Position.x = t1->PrevPosition.x + (500.f * std::min(p1->Mass / ((Physics*)leftObj->GetComponent(ComponentType::Physics))->Mass, 1.f) * engine->Get_Fixed_DT());
							((Transform*)leftObj->GetComponent(ComponentType::Transform))->Position.x = t1->Position.x - (((Rectangular*)b1)->width / 2.f) - (((Rectangular*)leftObj->GetComponent(ComponentType::Body))->width / 2.f) + 0.1f;
							((Physics*)leftObj->GetComponent(ComponentType::Physics))->IsBeingPushed = true;
						}
					}
					// Pushing the object
					else if (p1->Velocity.x < 0.0f) {
						// Make sure the pushed object is not colliding with another object on it's left
						if (((Rectangular*)leftObj->GetComponent(ComponentType::Body))->left_collision == nullptr) {
							// Will not make any sense for the velocity multiplier to exceed 1, 
							p1->Velocity.x *= std::min(p1->Mass / ((Physics*)leftObj->GetComponent(ComponentType::Physics))->Mass, 1.f);
							t1->Position.x = t1->PrevPosition.x + (p1->Velocity.x * engine->Get_Fixed_DT());
							((Transform*)leftObj->GetComponent(ComponentType::Transform))->Position.x = t1->Position.x - (((Rectangular*)b1)->width / 2.f) - (((Rectangular*)leftObj->GetComponent(ComponentType::Body))->width / 2.f);
							((Physics*)leftObj->GetComponent(ComponentType::Physics))->IsBeingPushed = true;
						}
						else {
							p1->Velocity.x = 0.0f;
							t1->Position.x = ((Rectangular*)((Rectangular*)b1)->left_collision->GetComponent(ComponentType::Body))->aabb.max.x + (((Rectangular*)b1)->width / 2);
						}
					}
				}
				else if (!(input::IsPressedRepeatedly(KEY::k) && input::IsPressedRepeatedly(KEY::a))) {
					p1->Velocity.x = 0.0f;
					t1->Position.x = ((Rectangular*)((Rectangular*)b1)->left_collision->GetComponent(ComponentType::Body))->aabb.max.x + (((Rectangular*)b1)->width / 2);
				}
			}
			if (((Rectangular*)b1)->collision_flag & COLLISION_RIGHT) {
				Object* rightObj = ((Rectangular*)b1)->right_collision;

				// For objects that are pushable
				if (p1->AbleToPushObjects && ((Rectangular*)rightObj->GetComponent(ComponentType::Body))->pushable) {
					// Pulling the object
					if (input::IsPressedRepeatedly(KEY::k) && input::IsPressedRepeatedly(KEY::d)) {
						if (((Rectangular*)b1)->left_collision == nullptr) {
							t1->Position.x = t1->PrevPosition.x - (500.f * std::min(p1->Mass / ((Physics*)rightObj->GetComponent(ComponentType::Physics))->Mass, 1.f) * engine->Get_Fixed_DT());
							((Transform*)rightObj->GetComponent(ComponentType::Transform))->Position.x = t1->Position.x + (((Rectangular*)b1)->width / 2.f) + (((Rectangular*)rightObj->GetComponent(ComponentType::Body))->width / 2.f) - 0.1f;
							((Physics*)rightObj->GetComponent(ComponentType::Physics))->IsBeingPushed = true;
						}
					}
					// Pushing the object
					else if (p1->Velocity.x > 0.0f) {
						// Make sure the pushed object is not colliding with another object on it's right
						if (((Rectangular*)rightObj->GetComponent(ComponentType::Body))->right_collision == nullptr) {
							// Will not make any sense for the velocity multiplier to exceed 1, 
							p1->Velocity.x *= std::min(p1->Mass / ((Physics*)rightObj->GetComponent(ComponentType::Physics))->Mass, 1.f);
							t1->Position.x = t1->PrevPosition.x + (p1->Velocity.x * engine->Get_Fixed_DT());
							((Transform*)rightObj->GetComponent(ComponentType::Transform))->Position.x = t1->Position.x + (((Rectangular*)b1)->width / 2.f) + (((Rectangular*)rightObj->GetComponent(ComponentType::Body))->width / 2.f);
							((Physics*)rightObj->GetComponent(ComponentType::Physics))->IsBeingPushed = true;
						}
						else {
							p1->Velocity.x = 0.0f;
							t1->Position.x = ((Rectangular*)((Rectangular*)b1)->right_collision->GetComponent(ComponentType::Body))->aabb.min.x - (((Rectangular*)b1)->width / 2);
						}
					}
				}
				else if (!(input::IsPressedRepeatedly(KEY::k) && input::IsPressedRepeatedly(KEY::d))) {
					p1->Velocity.x = 0.0f;
					t1->Position.x = ((Rectangular*)((Rectangular*)b1)->right_collision->GetComponent(ComponentType::Body))->aabb.min.x - (((Rectangular*)b1)->width / 2);
				}
			}
		}

		if (((Rectangular*)b1)->collision_flag)
			RecalculateBody(t1, b1);
	}
}

int total_grid_width, total_grid_height, num_of_grids_per_side;

/******************************************************************************
	PrepareUniformGrid
	- Prepares the uniform grid to insert objects into
*******************************************************************************/
void PrepareUniformGrid() {

	Vec2 level_size = engine->Get_End_Coords() - engine->Get_Start_Coords();

	int num_of_partitions_per_side = 6;

	//factor in objects that can be just outside the viewable area
	int extra_grids_per_side = num_of_partitions_per_side / 6;
	extra_grids_per_side = extra_grids_per_side ? extra_grids_per_side : 1;

	num_of_grids_per_side = num_of_partitions_per_side;// +(2 * extra_grids_per_side);
	total_grid_width = (int)level_size.x / num_of_partitions_per_side * num_of_grids_per_side;
	total_grid_height = (int)level_size.y / num_of_partitions_per_side * num_of_grids_per_side;

	Collision::uniform_grid.resize(num_of_grids_per_side);
	for (std::vector<std::vector<Object*>>& v : Collision::uniform_grid) {
		v.resize(num_of_grids_per_side);
	}
}

PhysicsSystem::PhysicsSystem() {

}

void PhysicsSystem::Initialize() {
	PrepareUniformGrid();
}

/******************************************************************************
	PhysicsSystem::Update
	- Main update loop for Physics system
*******************************************************************************/
void PhysicsSystem::Update() {

	// Do not update if the game is paused
	/*
	if (!engine->isPaused()) {

		accumulator += engine->GetDt();

		// Only run the physics code if fixed_dt has passed 
		if (accumulator < engine->Get_Fixed_DT()) {
			return;
		}

		// Check and see how many loops the physics needs to update
		while (accumulator > engine->Get_Fixed_DT()) {

			// To handle getting out of breakpoints
			if (num_of_steps >= 10) {
				accumulator = 0.0f;
				break;
			}

			num_of_steps++;
			accumulator -= engine->Get_Fixed_DT();
		}

	}*/

	if (engine->isPaused()) {
		return;
	}

	// Loop the physics code
	// Also make sure to handle getting out of breakpoints
	for (int i = 0; i < (engine->Get_NumOfSteps() > 10 ? 10 : engine->Get_NumOfSteps()); i++) {

		Collision::uniform_grid.clear();

		top_collision_cooldown = (top_collision_cooldown > 0.0f) ? top_collision_cooldown -= engine->Get_Fixed_DT() : 0.0f;

		/* Uniform grid */

		PrepareUniformGrid();

		/* Insert all objects into the grid */

		for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
			Body* b = (Body*)obj->second->GetComponent(ComponentType::Body);

			if (b == nullptr || !b->active)
				continue; // No physics or body in this object, move to next object

			b->inGrid.clear();

			if (b->GetShape() == Shape::Rectangle) {

				int w_index, h_index;
				Vec2 start_coord = engine->Get_Start_Coords();

				Vec2 p = ((Rectangular*)b)->aabb.P0();

				w_index = (((int)p.x - (int)start_coord.x) / (total_grid_width / num_of_grids_per_side));
				h_index = (((int)p.y - (int)start_coord.y) / (total_grid_height / num_of_grids_per_side));

				if (w_index >= 0 && w_index < num_of_grids_per_side && h_index >= 0 && h_index < num_of_grids_per_side)
					if (std::find(Collision::uniform_grid[w_index][h_index].begin(), Collision::uniform_grid[w_index][h_index].end(), obj->second) == Collision::uniform_grid[w_index][h_index].end()) {
						Collision::uniform_grid[w_index][h_index].push_back(obj->second);
						b->inGrid.push_back({ w_index, h_index });
					}

				p = ((Rectangular*)b)->aabb.P1();

				w_index = (((int)p.x - (int)start_coord.x) / (total_grid_width / num_of_grids_per_side));
				h_index = (((int)p.y - (int)start_coord.y) / (total_grid_height / num_of_grids_per_side));

				if (w_index >= 0 && w_index < num_of_grids_per_side && h_index >= 0 && h_index < num_of_grids_per_side)
					if (std::find(Collision::uniform_grid[w_index][h_index].begin(), Collision::uniform_grid[w_index][h_index].end(), obj->second) == Collision::uniform_grid[w_index][h_index].end()) {
						Collision::uniform_grid[w_index][h_index].push_back(obj->second);
						b->inGrid.push_back({ w_index, h_index });
					}

				p = ((Rectangular*)b)->aabb.P2();

				w_index = (((int)p.x - (int)start_coord.x) / (total_grid_width / num_of_grids_per_side));
				h_index = (((int)p.y - (int)start_coord.y) / (total_grid_height / num_of_grids_per_side));

				if (w_index >= 0 && w_index < num_of_grids_per_side && h_index >= 0 && h_index < num_of_grids_per_side)
					if (std::find(Collision::uniform_grid[w_index][h_index].begin(), Collision::uniform_grid[w_index][h_index].end(), obj->second) == Collision::uniform_grid[w_index][h_index].end()) {
						Collision::uniform_grid[w_index][h_index].push_back(obj->second);
						b->inGrid.push_back({ w_index, h_index });
					}

				p = ((Rectangular*)b)->aabb.P3();

				w_index = (((int)p.x - (int)start_coord.x) / (total_grid_width / num_of_grids_per_side));
				h_index = (((int)p.y - (int)start_coord.y) / (total_grid_height / num_of_grids_per_side));

				if (w_index >= 0 && w_index < num_of_grids_per_side && h_index >= 0 && h_index < num_of_grids_per_side)
					if (std::find(Collision::uniform_grid[w_index][h_index].begin(), Collision::uniform_grid[w_index][h_index].end(), obj->second) == Collision::uniform_grid[w_index][h_index].end()) {
						Collision::uniform_grid[w_index][h_index].push_back(obj->second);
						b->inGrid.push_back({ w_index, h_index });
					}
			}
		}


		// Update velocity for each object
		for (auto obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
			Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);

			if (p == nullptr)
				continue; // No physics in that object, move along

			// No acceleration, not needed in the game

			p->IsBeingPushed = false;

			// Apply gravity
			if (p->AffectedByGravity) {
				float acceleration = p->Force * (1.f / p->Mass) + gravity;
				p->Velocity.y += acceleration * engine->Get_Fixed_DT();
			}
			p->Velocity.y *= 0.999f; // Account for air resistance

			p->Force = 0.0f; // Reset force
		}

		// Loop through each object to update positiona and see if it's colliding with something
		for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj) {
			Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);
			Physics* p = (Physics*)obj->second->GetComponent(ComponentType::Physics);
			Body* b = (Body*)obj->second->GetComponent(ComponentType::Body);

			// For physics
			if (p != nullptr) {

				// Save current position to previous position
				t->PrevPosition = t->Position;

				if (p->Velocity.x == 0.f && p->Velocity.y == 0.f)
					continue; // No movement, so no need to calculate collision.

				// Calculate new position
				t->Position += p->Velocity * engine->Get_Fixed_DT();
			}

			if (b == nullptr)
				continue; // No body in this object or disabled, move to next object

			// Reset collision flags
			if (b->GetShape() == Shape::Rectangle)
				((Rectangular*)b)->ResetCollisionFlags();

			RecalculateBody(t, b);

			if (!b->active)
				continue; // Don't calculate collision for this object if active flag is unset

			bool collision_has_occured = false;

			for (const auto& grid : b->inGrid) {
				for (const auto& anotherobj : Collision::uniform_grid[grid.first][grid.second]) {
					if (obj->second == anotherobj)
						continue; // Can't collide with yourself

					Body* b2 = (Body*)anotherobj->GetComponent(ComponentType::Body);

					if (b2 == nullptr)
						continue; // No body in the other object, no way it's collidable

					if (Check_Collision(b, b2, engine->Get_Fixed_DT())) {
						collision_has_occured = true;
					}
				}
			}

			if (collision_has_occured && b->collision_response && p != nullptr)
				Response_Collision(t, b, p);
		}
	}
}

void PhysicsSystem::MessageRelay(Message_Handler* msg) {
	if (msg->GetMessage() == MessageID::Collide) {

	}
}
