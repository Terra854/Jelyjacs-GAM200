#include <Debug.h>
#include "Collision.h"

#include <algorithm>

namespace Collision {

	/*
		CIRCLE COLLISIONS
	*/

	/*
		This function calculates whether a given circle is colliding with a given line
	*/
	bool Check_Circle_Line(const Circle& circle,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime)
	{
		// LNS is a line segment with end points P0 and P1 and outward normal N.
		Vec2 P0 = lineSeg.Pt0();
		Vec2 P1 = lineSeg.Pt1();
		Vec2 N = lineSeg.Normal();

		N = Vec2Normalize(N); // N is normalized

		// Circle is centered by B and has radius R. It is moving with velocity V per one frame.
		// Bs is the starting position of B, Be is the end position of B, Bi is the intersection position of B(if any collision).
		float R = circle.radius;
		Vec2 Bs = circle.center;
		Vec2 Be = ptEnd;
		Vec2 V = Be - Bs;

		// Bs is starting from the inside half plane, and away from LNS by at least R
		// Here we consider we have an imaginary line LNS1, distant by �R (opposite N direction)
		if (Vec2DotProduct(N, Bs) - Vec2DotProduct(N, P0) <= -R) {
			// Check if the velocity vector V is within the end points of LNS1
			// Compute P0' and P1' to simulate LNS1 line edge points
			Vec2 P0_prime = P0 - R * N;
			Vec2 P1_prime = P1 - R * N;

			// M is the outward normal to Velocity V
			Vec2 M = { V.y, -V.x };
			M = Vec2Normalize(M);

			if (Vec2DotProduct(M, Bs - P0_prime) * Vec2DotProduct(M, Bs - P1_prime) < 0) {
				interTime = (Vec2DotProduct(N, P0) - Vec2DotProduct(N, Bs) - R) / Vec2DotProduct(N, V); //We are sure N.V != 0

				if (0 <= interTime && interTime <= 1) {
					interPt = Bs + V * interTime;
					normalAtCollision = { -N.x, -N.y }; // Normal of reflection is -N
					return true;
				}
			}
			else {
				return Check_Circle_LineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
			}
		}
		// Bs is starting from the outside half plane, and away from LNS by at least R
		// Here we consider we have an imaginary line LNS2 distant by +R (Same N direction)
		else if (Vec2DotProduct(N, Bs) - Vec2DotProduct(N, P0) >= R) {
			// Check if the velocity vector V is within the end points of LNS2
			// Compute P0' and P1' to simulate LNS2 line edge points
			Vec2 P0_prime = P0 + R * N;
			Vec2 P1_prime = P1 + R * N;

			// M is the outward normal to Velocity V
			Vec2 M = { V.y, -V.x };
			M = Vec2Normalize(M);

			if (Vec2DotProduct(M, Bs - P0_prime) * Vec2DotProduct(M, Bs - P1_prime) < 0) {
				interTime = (Vec2DotProduct(N, P0) - Vec2DotProduct(N, Bs) + R) / Vec2DotProduct(N, V); //We are sure N.V != 0

				if (0 <= interTime && interTime <= 1) {
					interPt = Bs + V * interTime;
					normalAtCollision = { N.x, N.y }; // Normal of reflection is N
					return true;
				}
			}
			else {
				return Check_Circle_LineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
			}
		}
		else { // The circle�s starting position Bs, is between both lines LNS1 and LNS2.
			return Check_Circle_LineEdge(true, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
		}
		return false; // no intersection
	}

	/*
		This function calculates whether a given circle is colliding with the edge of a given line
	*/
	bool Check_Circle_LineEdge(bool withinBothLines,
		const Circle& circle,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime)
	{
		Vec2 Bs = circle.center;

		Vec2 V = ptEnd - Bs;
		Vec2 normalizedV = Vec2Normalize(V);

		Vec2 P0 = lineSeg.Pt0();
		Vec2 P1 = lineSeg.Pt1();
		float R = circle.radius;
		Vec2 BsP0 = P0 - Bs;
		Vec2 BsP1 = P1 - Bs;
		Vec2 P0P1 = P1 - P0;

		Vec2 M = { V.y, -V.x };
		M = Vec2Normalize(M); // Only normalized M is needed

		float m, s, ti;

		if (withinBothLines) {// When it�s true, is to say that Bs is starting from between both imaginary lines
			// Check which edge may collide first
			if (Vec2DotProduct(BsP0, P0P1) > 0) { // P0 side
				m = Vec2DotProduct(BsP0, normalizedV);
				if (m > 0) { //Otherwise no collision

					// Reaching here means the circle movement is facing P0
					// M is normalized outward normal of V
					float dist0 = Vec2DotProduct(BsP0, M); // Same as P0.M - Bs.M (Shortest distance from P0 to V)
					if (abs(dist0) > R)
						return false; // no collision

					// Reaching here means the circle movement is going towards P0
					// The next line assumes the circle at collision time with P0
					s = sqrt(R * R - dist0 * dist0);
					ti = (m - s) / Vec2Length(V);

					if (ti <= 1) {
						interPt = Bs + V * ti;

						// Normal of reflection is P0Bi normalized
						Vec2 P0Bi = interPt - P0;
						normalAtCollision = Vec2Normalize(P0Bi);
						interTime = ti;
						return true;
					}
				}
			}
			else { // P1 side
				m = Vec2DotProduct(BsP1, normalizedV);
				if (m > 0) { // Otherwise no collision

					// Reaching here means the circle movement is facing P1
					// M is normalized outward normal of V
					float dist1 = Vec2DotProduct(BsP1, M); //Same as P1.M - Bs.M
					if (abs(dist1) > R)
						return false; // no collision

					// Reaching here means the circle movement is going towards P1
					// The next line assumes the circle at collision time with P1
					s = sqrt(R * R - dist1 * dist1);
					ti = (m - s) / Vec2Length(V);

					if (ti <= 1) {
						interPt = Bs + V * ti;

						// Normal of reflection is P1Bi normalized
						Vec2 P1Bi = interPt - P1;
						normalAtCollision = Vec2Normalize(P1Bi);
						interTime = ti;
						return true;
					}
				}
			}
		}
		else { // not withinBothLines
			// Check which line edge, P0 or P1, is closer to the velocity vector V?
			bool P0Side;
			float dist0 = Vec2DotProduct(BsP0, M); // Same as P0.M - Bs.M (M is normalized outward normal of V)
			float dist1 = Vec2DotProduct(BsP1, M); // Same as P1.M - Bs.M
			float dist0_absoluteValue = abs(dist0);
			float dist1_absoluteValue = abs(dist1);

			if (dist0_absoluteValue > R && dist1_absoluteValue > R)
				return false; // No Collision
			else if (dist0_absoluteValue <= R && dist1_absoluteValue <= R) {
				float m0 = Vec2DotProduct(BsP0, normalizedV);
				float m1 = Vec2DotProduct(BsP1, normalizedV);
				float m0_absoluteValue = abs(m0);
				float m1_absoluteValue = abs(m1);

				P0Side = (m0_absoluteValue < m1_absoluteValue);
			}
			else {
				P0Side = (dist0_absoluteValue <= R);
			}

			if (P0Side) { // circle is closer to P0
				m = Vec2DotProduct(BsP0, normalizedV);
				if (m < 0)
					return false; // moving away

				// Reaching here means the circle movement is going towards P0
				// The next line assumes the circle at collision time with P0
				dist0 = Vec2DotProduct(BsP0, M);
				s = sqrt(R * R - dist0 * dist0);
				ti = (m - s) / Vec2Length(V);

				if (ti <= 1) {
					interPt = Bs + V * ti;

					// Normal of reflection is P0Bi normalized
					Vec2 P0Bi = interPt - P0;
					normalAtCollision = Vec2Normalize(P0Bi);
					interTime = ti;
					return true;
				}
			}
			else { // circle is closer to P1
				m = Vec2DotProduct(BsP1, normalizedV);
				if (m < 0)
					return false; // moving away

				dist1 = Vec2DotProduct(BsP1, M);
				s = sqrt(R * R - dist1 * dist1);
				ti = (m - s) / Vec2Length(V);

				if (ti <= 1) {
					interPt = Bs + V * ti;

					// Normal of reflection is P1Bi normalized
					Vec2 P1Bi = interPt - P1;
					normalAtCollision = Vec2Normalize(P1Bi);
					interTime = ti;
					return true;
				}
			}
		}

		return false;// no collision
	}

	/*
		RECTANGLE COLLISIONS
	*/

	/*
		This function checks if a given rectangle intersects with a line via AABB.
		NOTE: Currently untested, so don't know if it works atm.
	*/
	bool Check_AABB_Line(const AABB& aabb,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime)
	{
		// LNS is a line segment with end points P0 and P1 and outward normal N.

		Vec2 P0 = lineSeg.Pt0();
		Vec2 P1 = lineSeg.Pt1();
		Vec2 N = lineSeg.Normal();

		N = Vec2Normalize(N); // N is normalized

		// Split the AABB into 4 lines representing the 4 edges
		Line edges[4] = {
			{ aabb.P0(), aabb.P1() },
			{ aabb.P1(), aabb.P2() },
			{ aabb.P2(), aabb.P3() },
			{ aabb.P3(), aabb.P0() }
		};

		// Check each edge of the AABB against the provided line segment for collision
		for (int i = 0; i < 4; i++)
		{
			P0 = edges[i].Pt0();
			P1 = edges[i].Pt1();

			// Determine the vector of movement based on the difference between ptEnd and the current edge's start
			Vec2 V = ptEnd - P0;

			// Check if the line intersects with the bottom side of the AABB
			if (Vec2DotProduct(N, P0) - Vec2DotProduct(N, aabb.P0()) <= 0)
			{
				// Check if the intersection lies between the end points of the current edge
				if (Vec2DotProduct(V, P0) * Vec2DotProduct(V, P1) < 0)
				{
					interTime = (Vec2DotProduct(N, aabb.P0()) - Vec2DotProduct(N, P0)) / Vec2DotProduct(N, V);

					if (0 <= interTime && interTime <= 1)
					{
						interPt = P0 + V * interTime;
						normalAtCollision = { -N.x, -N.y }; // Normal of reflection is -N
						return true;
					}
				}
			}
			// Check if the line intersects with the top side of the AABB
			else if (Vec2DotProduct(N, P0) - Vec2DotProduct(N, aabb.P0()) >= 0)
			{
				// Check if the intersection lies between the end points of the current edge
				if (Vec2DotProduct(V, P0) * Vec2DotProduct(V, P1) < 0)
				{
					interTime = (Vec2DotProduct(N, aabb.P0()) - Vec2DotProduct(N, P0)) / Vec2DotProduct(N, V);

					if (0 <= interTime && interTime <= 1)
					{
						interPt = P0 + V * interTime;
						normalAtCollision = { N.x, N.y }; // Normal of reflection is N
						return true;
					}
				}
			}
		}
		return false;  // no intersection
	}

	bool PointLineCollision(const Vec2& point, const Line& line) {
		// Get the line segment vector and the point-to-start vector
		Vec2 lineVec = line.Pt1() - line.Pt0();
		Vec2 pointVec = point - line.Pt0();

		// Compute the projection of pointVec onto lineVec
		float projLength = pointVec.x * lineVec.x + pointVec.y * lineVec.y;
		if (projLength < 0 || projLength > lineVec.x * lineVec.x + lineVec.y * lineVec.y)
			return false;  // point is outside the line segment

		float projLenPerp = pointVec.x * lineVec.y - pointVec.y * lineVec.x;
		// Here, you can use a threshold to check for "close enough" collision.
		return abs(projLenPerp) <= 1.0e-6; // Very small threshold
	}

	/*
		This function checks which side of a given rectangle intersects with a line via hotspot checking
		via PointLineCollision

		Should use Check_AABB_Line first to see if collision actually occured

		NOTE: Currently untested, so don't know if it works atm.
		TODO: Check for collision at the corners
	*/
	int Check_Rect_Line(float PosX, float PosY, float scaleX, float scaleY, const Line& line)
	{
		Vec2 point;
		int flag = 0;

		/************************
			Left
		************************/
		point.x = PosX - scaleX / 2;
		point.y = PosY + scaleY / 4;

		if (PointLineCollision(point, line)) {
			flag |= COLLISION_LEFT;
		}

		point.y = PosY - scaleY / 4;
		if (PointLineCollision(point, line)) {
			flag |= COLLISION_LEFT;
		}

		/************************
			Right
		************************/
		point.x = PosX + scaleX / 2;
		point.y = PosY + scaleY / 4;

		if (PointLineCollision(point, line)) {
			flag |= COLLISION_RIGHT;
		}

		point.y = PosY - scaleY / 4;
		if (PointLineCollision(point, line)) {
			flag |= COLLISION_RIGHT;
		}

		/************************
			Top
		************************/
		point.x = PosX + scaleX / 4;
		point.y = PosY + scaleY / 2;

		if (PointLineCollision(point, line)) {
			flag |= COLLISION_TOP;
		}

		point.x = PosX - scaleX / 4;
		if (PointLineCollision(point, line)) {
			flag |= COLLISION_TOP;
		}

		/************************
			Bottom
		************************/
		point.x = PosX + scaleX / 4;
		point.y = PosY - scaleY / 2;

		if (PointLineCollision(point, line)) {
			flag |= COLLISION_BOTTOM;
		}

		point.x = PosX - scaleX / 4;
		if (PointLineCollision(point, line)) {
			flag |= COLLISION_BOTTOM;
		}

		return flag;
	}

	/*
		This function checks if a given rectangle intersects with another rectangle via AABB.
		NOTE: Currently not working as it needs to get frame time which is not implemented atm.
	*/
	bool Check_AABB_AABB(const AABB& aabb1, const Vec2& vel1,
		const AABB& aabb2, const Vec2& vel2)
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
			float tFirst = 0, tLast = 0; // tLast = 0 is placeholder, will need to change to frame time when it gets implemented

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
		This function calculates the new direction of a rectangle or circle after colliding with a line
	*/
	void Response_Object_Line(const Vec2& ptInter,
		const Vec2& normal,
		Vec2& ptEnd,
		Vec2& reflected)
	{
		Vec2 penetration = ptEnd - ptInter;
		reflected = penetration - 2.f * Vec2DotProduct(penetration, normal) * normal;

		reflected = Vec2Normalize(reflected);

		ptEnd = ptInter + reflected * Vec2Length(penetration);
	}
}