/* !
@file	Vec2.cpp
@author	Tan Yee Ann
@date	28/9/2023

This file contains the definitions of the member functions in the Vec2 class
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Vec2.h"

#include <cmath>

/******************************************************************************
	Add a vector to this vector
*******************************************************************************/
Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

/******************************************************************************
	Subtract a vector from this vector
*******************************************************************************/
Vec2& Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

/******************************************************************************
	Multiply a vector by a given magnitude
*******************************************************************************/
Vec2& Vec2::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;

	return *this;
}

/******************************************************************************
	Divide a vector by a given magnitude
*******************************************************************************/
Vec2& Vec2::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;

	return *this;
}

/******************************************************************************
	Returns a negative version of this vector
*******************************************************************************/
Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}

/******************************************************************************
	Print this vector to a given output
*******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

/******************************************************************************
	Add 2 vectors together
*******************************************************************************/
Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
{
	return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

/******************************************************************************
	Subtracts a vector from another vector
*******************************************************************************/
Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
	return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

/******************************************************************************
	Multiply a given vector by a given magnitude
*******************************************************************************/
Vec2 operator*(const Vec2& lhs, float rhs)
{
	return Vec2(lhs.x * rhs, lhs.y * rhs);
}

/******************************************************************************
	Multiply a given vector by a given magnitude
*******************************************************************************/
Vec2 operator*(float lhs, const Vec2& rhs)
{
	return Vec2(lhs * rhs.x, lhs * rhs.y);
}

/******************************************************************************
	Divide a given vector by a given magnitude
*******************************************************************************/
Vec2 operator/(const Vec2& lhs, float rhs)
{
	return Vec2(lhs.x / rhs, lhs.y / rhs);
}

/******************************************************************************
	Check to see if the 2 given vectors are equal
*******************************************************************************/
bool operator==(const Vec2& lhs, const Vec2& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

/******************************************************************************
	Check to see if the 2 given vectors are not equal
*******************************************************************************/
bool operator!=(const Vec2& lhs, const Vec2& rhs)
{
	return (lhs.x != rhs.x || lhs.y != rhs.y);
}

/******************************************************************************
	Converts the vector for use in OpenGL calls
*******************************************************************************/
glm::vec2 Vec2::ToGlmVec2() {
	return glm::vec2(x, y);
}

ImVec2 Vec2::ToImVec2()
{
	return ImVec2(x, y);
}

/******************************************************************************
	Normalize a given vector
*******************************************************************************/
Vec2 Vec2Normalize(const Vec2& v)
{
	float f = Vec2Length(v);
	return Vec2(v.x / f, v.y / f);
}

/******************************************************************************
	Calculates the length of a given vector
*******************************************************************************/
float Vec2Length(const Vec2& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

/******************************************************************************
	Calculates the square length of a given vector
*******************************************************************************/
float Vec2SquareLength(const Vec2& v)
{
	return Vec2Length(v) * Vec2Length(v);
}

/******************************************************************************
	Calculates the distance between 2 vectors
*******************************************************************************/
float Vec2Distance(const Vec2& v0, const Vec2& v1)
{
	Vec2 distance{ v1.x - v0.x, v1.y - v0.y };
	return Vec2Length(distance);
}

/******************************************************************************
	Calculates the square distance between 2 vectors
*******************************************************************************/
float Vec2SquareDistance(const Vec2& v0, const Vec2& v1)
{
	Vec2 distance{ v1.x - v0.x, v1.y - v0.y };
	return Vec2Length(distance) * Vec2Length(distance);
}

/******************************************************************************
	Calculates the dot product of 2 vectors
*******************************************************************************/
float Vec2DotProduct(const Vec2& v0, const Vec2& v1)
{
	return v1.x * v0.x + v1.y * v0.y;
}

/******************************************************************************
	Calculates the cross product magnitude of 2 vectors
*******************************************************************************/
float Vec2CrossProductMag(const Vec2& v0, const Vec2& v1)
{
	return v0.x * v1.y - v0.y * v1.x;
}