/* !
@file	Vec4.cpp
@author	Tan Yee Ann
@date	21/11/2023

This file contains the definitions of the member functions in the Vec4 class
*//*__________________________________________________________________________*/

#include "Vec4.h"

/******************************************************************************
	Add a vector to this vector
*******************************************************************************/
Vec4& Vec4::operator+=(const Vec4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	a += rhs.a;

	return *this;
}

/******************************************************************************
	Subtract a vector from this vector
*******************************************************************************/
Vec4& Vec4::operator-=(const Vec4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	a -= rhs.a;

	return *this;
}

/******************************************************************************
	Multiply a vector by a given magnitude
*******************************************************************************/
Vec4& Vec4::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	a *= rhs;

	return *this;
}

/******************************************************************************
	Divide a vector by a given magnitude
*******************************************************************************/
Vec4& Vec4::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	a /= rhs;

	return *this;
}

/******************************************************************************
	Returns a negative version of this vector
*******************************************************************************/
Vec4 Vec4::operator-() const
{
	return Vec4(-x, -y, -z, -a);
}

/******************************************************************************
	Print this vector to a given output
*******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Vec4& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.a << ")";
	return os;
}

/******************************************************************************
	Add 2 vectors together
*******************************************************************************/
Vec4 operator+(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.a + rhs.a);
}

/******************************************************************************
	Subtracts a vector from another vector
*******************************************************************************/
Vec4 operator-(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.a - rhs.a);
}

/******************************************************************************
	Multiply 2 vectors together
*******************************************************************************/
Vec4 operator*(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.a * rhs.a);
}

/******************************************************************************
	Multiply a given vector by a given magnitude
*******************************************************************************/
Vec4 operator*(const Vec4& lhs, float rhs)
{
	return Vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.a * rhs);
}

/******************************************************************************
	Multiply a given vector by a given magnitude
*******************************************************************************/
Vec4 operator*(float lhs, const Vec4& rhs)
{
	return Vec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.a);
}

/******************************************************************************
	Divide a given vector by another vector
*******************************************************************************/
Vec4 operator/(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.a / rhs.a);
}

/******************************************************************************
	Divide a given vector by a given magnitude
*******************************************************************************/
Vec4 operator/(const Vec4& lhs, float rhs)
{
	return Vec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.a / rhs);
}

/******************************************************************************
	Check to see if the 2 given vectors are equal
*******************************************************************************/
bool operator==(const Vec4& lhs, const Vec4& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.a == rhs.a);
}

/******************************************************************************
	Check to see if the 2 given vectors are not equal
*******************************************************************************/
bool operator!=(const Vec4& lhs, const Vec4& rhs)
{
	return (lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z && lhs.a != rhs.a);
}

/******************************************************************************
	Converts the vector for use in ImGui
*******************************************************************************/
ImVec4 Vec4::ToImVec4() {
	return ImVec4(x, y, z, a);
}