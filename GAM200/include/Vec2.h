#pragma once
#include <Debug.h>

#include <glm/glm.hpp>

struct Vec2
{
	float x, y;

	// Constructors
	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float _x, float _y);

	Vec2& operator=(const Vec2& rhs) = default;
	Vec2(const Vec2& rhs) = default;

	// Assignment operators
	Vec2& operator += (const Vec2& rhs);
	Vec2& operator -= (const Vec2& rhs);
	Vec2& operator *= (float rhs);
	Vec2& operator /= (float rhs);

	// Unary operators
	Vec2 operator -() const;

	// To be used with OpenGL calls only
	glm::vec2 ToGlmVec2();
};

// Binary operators
Vec2 operator+ (const Vec2& lhs, const Vec2& rhs);
Vec2 operator- (const Vec2& lhs, const Vec2& rhs);
Vec2 operator* (const Vec2& lhs, float rhs);
Vec2 operator* (float lhs, const Vec2& rhs);
Vec2 operator/ (const Vec2& lhs, float rhs);

// Comparison operators
bool operator== (const Vec2& lhs, const Vec2& rhs);
bool operator!= (const Vec2& lhs, const Vec2& rhs);

Vec2 Vec2Normalize(const Vec2& pVec0);

float Vec2Length(const Vec2& pVec0);

float Vec2SquareLength(const Vec2& pVec0);

float Vec2Distance(const Vec2& pVec0, const Vec2& pVec1);

float Vec2SquareDistance(const Vec2& pVec0, const Vec2& pVec1);

float Vec2DotProduct(const Vec2& pVec0, const Vec2& pVec1);

float Vec2CrossProductMag(const Vec2& pVec0, const Vec2& pVec1);

