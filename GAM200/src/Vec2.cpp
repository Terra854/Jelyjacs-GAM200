/* !
@file
@author	t.yeeann@digipen.edu
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Vec2.h"

#include <cmath>

Vec2::Vec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

Vec2& Vec2::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;

	return *this;
}

Vec2& Vec2::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;

	return *this;
}

Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}

std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
{
	return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
	return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vec2 operator*(const Vec2& lhs, float rhs)
{
	return Vec2(lhs.x * rhs, lhs.y * rhs);
}

Vec2 operator*(float lhs, const Vec2& rhs)
{
	return Vec2(lhs * rhs.x, lhs * rhs.y);
}

Vec2 operator/(const Vec2& lhs, float rhs)
{
	return Vec2(lhs.x / rhs, lhs.y / rhs);
}

bool operator==(const Vec2& lhs, const Vec2& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool operator!=(const Vec2& lhs, const Vec2& rhs)
{
	return (lhs.x != rhs.x || lhs.y != rhs.y);
}

glm::vec2 Vec2::ToGlmVec2() {
	return glm::vec2(x, y);
}

Vec2 Vec2Normalize(const Vec2& pVec0)
{
	float f = Vec2Length(pVec0);
	return Vec2(pVec0.x / f, pVec0.y / f);
}

float Vec2Length(const Vec2& pVec0)
{
	return sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y);
}

float Vec2SquareLength(const Vec2& pVec0)
{
	return Vec2Length(pVec0) * Vec2Length(pVec0);
}

float Vec2Distance(const Vec2& pVec0, const Vec2& pVec1)
{
	Vec2 distance{ pVec1.x - pVec0.x, pVec1.y - pVec0.y };
	return Vec2Length(distance);
}

float Vec2SquareDistance(const Vec2& pVec0, const Vec2& pVec1)
{
	Vec2 distance{ pVec1.x - pVec0.x, pVec1.y - pVec0.y };
	return Vec2Length(distance) * Vec2Length(distance);
}

float Vec2DotProduct(const Vec2& pVec0, const Vec2& pVec1)
{
	return pVec1.x * pVec0.x + pVec1.y * pVec0.y;
}

float Vec2CrossProductMag(const Vec2& pVec0, const Vec2& pVec1)
{
	return pVec0.x * pVec1.y - pVec0.y * pVec1.x;
}