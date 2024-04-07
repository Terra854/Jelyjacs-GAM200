/* !
@file	Collision.cpp
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains functions to calculate collisions between the different game objects
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Collision.h"

#include <algorithm>
#include <components/Body.h>

namespace Collision {

	/*
		UNIFORM GRID
	*/

	// 1st vector is for width
	// 2nd vector is for height
	// 3rd vector is for all the objects inside the grid
	std::vector<std::vector<std::vector<Object*>>> uniform_grid;

	/*
		RECTANGLE COLLISIONS
	*/

	/*
		This function checks if a given rectangle intersects with another rectangle via AABB.
	*/
	bool Check_AABB_AABB(const AABB& aabb1, const Vec2& vel1,
		const AABB& aabb2, const Vec2& vel2, float dt)
	{
		/*

		The steps are:
		Step 1: Check for collision detection between rectangles (assuming static here).
			If the check returns no overlap you continue with the following steps.

		Step 2: Initialize and calculate the new velocity of Vb
			tFirst = 0
			tLast = dt

		Step 3: Working with one dimension (x-axis).
			if(Vb < 0)
				case 1
				case 4
			if(Vb > 0)
				case 2
				case 3

			case 5

		Step 4: Repeat step 3 on the y-axis

		Step 5: Otherwise the rectangles intersect

		*/
		
		// Step 1
		if (aabb1.max.x < aabb2.min.x || aabb1.max.y < aabb2.min.y || aabb1.min.x > aabb2.max.x || aabb1.min.y > aabb2.max.y) {

			// Step 2
			Vec2 Vb = { vel2.x - vel1.x, vel2.y - vel1.y };
			float tFirst = 0, tLast = dt;

			// Step 3
			if (Vb.x < 0) {
				if (aabb1.min.x > aabb2.max.x) // Case 1
					return false;
				if (aabb1.max.x < aabb2.min.x) // Case 4
					tFirst = std::max((aabb1.max.x - aabb2.min.x) / Vb.x, tFirst);
				if (aabb1.min.x < aabb2.max.x) // Case 4
					tLast = std::min((aabb1.min.x - aabb2.max.x) / Vb.x, tLast);
			}
			if (Vb.x > 0) {
				if (aabb1.min.x > aabb2.max.x) // Case 2
					tFirst = std::max((aabb1.min.x - aabb2.max.x) / Vb.x, tFirst);
				if (aabb1.max.x > aabb2.min.x) // Case 2
					tLast = std::min((aabb1.max.x - aabb2.min.x) / Vb.x, tLast);
				if (aabb1.max.x < aabb2.min.x) // Case 3
					return false;
			}
			if (tFirst > tLast) // Case 5
				return false;

			// Step 4
			if (Vb.y < 0) {
				if (aabb1.min.y > aabb2.max.y) // Case 1
					return false;
				if (aabb1.max.y < aabb2.min.y) // Case 4
					tFirst = std::max((aabb1.max.y - aabb2.min.y) / Vb.y, tFirst);
				if (aabb1.min.y < aabb2.max.y) // Case 4
					tLast = std::min((aabb1.min.y - aabb2.max.y) / Vb.y, tLast);
			}
			if (Vb.y > 0) {
				if (aabb1.min.y > aabb2.max.y) // Case 2
					tFirst = std::max((aabb1.min.y - aabb2.max.y) / Vb.y, tFirst);
				if (aabb1.max.y > aabb2.min.y) // Case 2
					tLast = std::min((aabb1.max.y - aabb2.min.y) / Vb.y, tLast);
				if (aabb1.max.y < aabb2.min.y) // Case 3
					return false;
			}
			if (tFirst > tLast) // Case 5
				return false;
		}
		// Step 5
		return true;
		
	}
	
	/*
		This function checks to see if a point is colliding with a rectangle
	*/
	bool PointRectCollision(const Vec2& point, const Rectangular* rect)
	{
		// Using the AABB properties to determine the boundaries
		return (point.x >= rect->aabb.min.x && point.x <= rect->aabb.max.x
			&& point.y >= rect->aabb.min.y && point.y <= rect->aabb.max.y);
	}

	/*
		This function checks which side of a given rectangle intersects with another rectangle via hotspot checking
		via PointRectCollision using AABB data

		Should use Check_AABB_AABB first to see if collision actually occured first as using this function directly
		can be compute intensive when processing hundreds of objects
	*/
	void Check_Rect_Rect(Rectangular* rect1, Rectangular* rect2)
	{
		Vec2 point;
		int flag = 0;

		int num_hotspots = 3; // Number of hotspots to check, applies to all sides

		float height = rect1->aabb.max.y - rect1->aabb.min.y;
		float length = rect1->aabb.max.x - rect1->aabb.min.x;

		/************************
			Left
		************************/
		point.x = rect1->aabb.min.x;

		for (int i = 1; i <= num_hotspots; i++) {
			point.y = rect1->aabb.min.y + (height / (num_hotspots + 1) * i);
			if (PointRectCollision(point, rect2)) {
				flag |= COLLISION_LEFT;
				rect1->left_collision = rect2->GetOwner();
				break;
			}
		}

		/************************
			Right
		************************/
		point.x = rect1->aabb.max.x;

		for (int i = 1; i <= num_hotspots; i++) {
			point.y = rect1->aabb.min.y + (height / (num_hotspots + 1) * i);
			if (PointRectCollision(point, rect2)) {
				flag |= COLLISION_RIGHT;
				rect1->right_collision = rect2->GetOwner();
				break;
			}
		}

		/************************
			Top
		************************/
		point.y = rect1->aabb.max.y;

		for (int i = 1; i <= num_hotspots; i++) {
			point.x = rect1->aabb.min.x + (length / (num_hotspots + 1) * i);
			if (PointRectCollision(point, rect2)) {
				flag |= COLLISION_TOP;
				rect1->top_collision = rect2->GetOwner();
				break;
			}
		}

		/************************
			Bottom
		************************/
		point.y = rect1->aabb.min.y;

		for (int i = 1; i <= num_hotspots; i++) {
			point.x = rect1->aabb.min.x + (length / (num_hotspots + 1) * i);
			if (PointRectCollision(point, rect2)) {
				flag |= COLLISION_BOTTOM;
				rect1->bottom_collision = rect2->GetOwner();
				break;
			}
		}

		rect1->collision_flag |= flag;
	}

	bool IsObjectInsideLaser(Rectangular* obj_b, Object* anotherObj)
	{

		if (obj_b == nullptr || anotherObj == nullptr) {
			return false;
		}

		return (obj_b->left_collision == anotherObj && obj_b->right_collision == anotherObj) || obj_b->top_collision == anotherObj || obj_b->bottom_collision == anotherObj;
	}
}