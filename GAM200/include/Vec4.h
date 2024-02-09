#pragma once
/* !
@file	Vec4.h
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains the declaration for the Vec4 class used to represent a 4D vector,
mostly used to store color data.
*//*__________________________________________________________________________*/
#include <Debug.h>

#include <ImGui/imgui.h>
#include <ostream>

struct Vec4
{
	float x, y, z, a;

	// Constructors
	Vec4() : x(0.0f), y(0.0f), z(0.0f), a(0.0f) {}
	Vec4(float _x, float _y, float _z, float _a) : x(_x), y(_y), z(_z), a(_a) {}

	Vec4& operator=(const Vec4& rhs) = default;
	Vec4(const Vec4& rhs) = default;

	// Assignment operators
	Vec4& operator += (const Vec4& rhs);
	Vec4& operator -= (const Vec4& rhs);
	Vec4& operator *= (float rhs);
	Vec4& operator /= (float rhs);

	// Unary operators
	Vec4 operator -() const;

	// Converts the vector for use with ImGui
	ImVec4 ToImVec4();

	// For std::cout << Vec4_variable
	friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

// Binary operators
Vec4 operator+ (const Vec4& lhs, const Vec4& rhs);
Vec4 operator- (const Vec4& lhs, const Vec4& rhs);
Vec4 operator* (const Vec4& lhs, const Vec4& rhs);
Vec4 operator* (const Vec4& lhs, float rhs);
Vec4 operator* (float lhs, const Vec4& rhs);
Vec4 operator/ (const Vec4& lhs, const Vec4& rhs);
Vec4 operator/ (const Vec4& lhs, float rhs);

// Comparison operators
bool operator== (const Vec4& lhs, const Vec4& rhs);
bool operator!= (const Vec4& lhs, const Vec4& rhs);