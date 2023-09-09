#include "Collision.h"

namespace Collision {

	/*
		This function calculates whether a given circle is colliding with a given line
	*/
	int Check_Circle_Line(const Circle& circle,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime,
		bool& checkLineEdges)
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
		// Here we consider we have an imaginary line LNS1, distant by –R (opposite N direction)
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
					normalAtCollision = { -N.x, -N.y }; //Normal of reflection is -N
					return 1;
				}
			}
			else {
				if (checkLineEdges)
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
					return 1;
				}
			}
			else {
				if (checkLineEdges)
					return Check_Circle_LineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
			}
		}
		else { // The circle’s starting position Bs, is between both lines LNS1 and LNS2.
			if (checkLineEdges)
				return Check_Circle_LineEdge(true, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
		}
		return 0; // no intersection
	}

	/*
		This function calculates whether a given circle is colliding with the edge of a given line
	*/
	int Check_Circle_LineEdge(bool withinBothLines,
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

		if (withinBothLines) {// When it’s true, is to say that Bs is starting from between both imaginary lines
			// Check which edge may collide first
			if (Vec2DotProduct(BsP0, P0P1) > 0) { // P0 side
				m = Vec2DotProduct(BsP0, normalizedV);
				if (m > 0) { //Otherwise no collision

					// Reaching here means the circle movement is facing P0
					// M is normalized outward normal of V
					float dist0 = Vec2DotProduct(BsP0, M); // Same as P0.M - Bs.M (Shortest distance from P0 to V)
					if (abs(dist0) > R)
						return 0; // no collision

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
						return 1;
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
						return 0; // no collision

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
						return 1;
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
				return 0; // No Collision
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
					return 0; // moving away

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
					return 1;
				}
			}
			else { // circle is closer to P1
				m = Vec2DotProduct(BsP1, normalizedV);
				if (m < 0)
					return 0; // moving away

				dist1 = Vec2DotProduct(BsP1, M);
				s = sqrt(R * R - dist1 * dist1);
				ti = (m - s) / Vec2Length(V);

				if (ti <= 1) {
					interPt = Bs + V * ti;

					// Normal of reflection is P1Bi normalized
					Vec2 P1Bi = interPt - P1;
					normalAtCollision = Vec2Normalize(P1Bi);
					interTime = ti;
					return 1;
				}
			}
		}

		return 0;// no collision
	}

	/*
		This function calculates the new direction of the circle after colliding with a line
	*/
	void Response_Circle_Line(const Vec2& ptInter,
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