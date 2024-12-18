#pragma once
/* !
@file	Vec2.h
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains the declaration for the Vec2 class used to represent a 2D vector
*//*__________________________________________________________________________*/
#include <Debug.h>

#include <glm/glm.hpp>
#include <ostream>
#include <ImGui/imgui.h>

struct Vec2
{
	float x, y;

	// Constructors
	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float _x, float _y) : x(_x), y(_y) {}
	Vec2(ImVec2 v) : x(v.x), y(v.y) {}

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

	// To be used with ImGui calls
	ImVec2 ToImVec2();

	// For std::cout << vec2_variable
	friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
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

Vec2 Vec2Normalize(const Vec2& v);

float Vec2Length(const Vec2& v);

float Vec2SquareLength(const Vec2& v);

float Vec2Distance(const Vec2& v0, const Vec2& v1);

float Vec2SquareDistance(const Vec2& v0, const Vec2& v1);

float Vec2DotProduct(const Vec2& v0, const Vec2& v1);

float Vec2CrossProductMag(const Vec2& v0, const Vec2& v1);

