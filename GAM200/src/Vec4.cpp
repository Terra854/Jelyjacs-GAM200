/* !
@file	Vec4.cpp
@author	Tan Yee Ann
@date	21/11/2023

This file contains the definitions of the member functions in the Vec4 class
*//*__________________________________________________________________________*/

#include "Vec4.h"

Vec4& Vec4::operator+=(const Vec4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	a += rhs.a;

	return *this;
}

Vec4& Vec4::operator-=(const Vec4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	a -= rhs.a;

	return *this;
}

Vec4& Vec4::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	a *= rhs;

	return *this;
}

Vec4& Vec4::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	a /= rhs;

	return *this;
}

Vec4 Vec4::operator-() const
{
	return Vec4(-x, -y, -z, -a);
}

// Mostly for debugging via std::cout
std::ostream& operator<<(std::ostream& os, const Vec4& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.a << ")";
	return os;
}

Vec4 operator+(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.a + rhs.a);
}

Vec4 operator-(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.a - rhs.a);
}

Vec4 operator*(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.a * rhs.a);
}

Vec4 operator*(const Vec4& lhs, float rhs)
{
	return Vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.a * rhs);
}

Vec4 operator*(float lhs, const Vec4& rhs)
{
	return Vec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.a);
}

Vec4 operator/(const Vec4& lhs, const Vec4& rhs)
{
	return Vec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.a / rhs.a);
}

Vec4 operator/(const Vec4& lhs, float rhs)
{
	return Vec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.a / rhs);
}

bool operator==(const Vec4& lhs, const Vec4& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.a == rhs.a);
}

bool operator!=(const Vec4& lhs, const Vec4& rhs)
{
	return (lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z && lhs.a != rhs.a);
}

// Converts the vector for use with ImGui
ImVec4 Vec4::ToImVec4() {
	return ImVec4(x, y, z, a);
}