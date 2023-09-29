#pragma once
/* !
@file	Mat3.h
@author	Tan Yee Ann
@date	28/9/2023

This file contains the declaration for the Mat3 class used to represent a 3x3 matrix
*//*__________________________________________________________________________*/
#include <Debug.h>

#include "Vec2.h"

struct Mat3
{
	#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
	#pragma warning( disable : 4201 )
	#endif
	union {
		struct
		{
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
		};

		float m[9];
		float m2[3][3];
	};

	Mat3() : m{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}
	Mat3(const float* pArr);
	Mat3(float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22);
	Mat3& operator=(const Mat3& rhs);

	Mat3(const Mat3& rhs) = default;

	// Assignment operators
	Mat3& operator*=(const Mat3& rhs);

	// To be used with OpenGL calls only
	glm::mat3 ToGlmMat3();
};

Mat3 operator* (const Mat3& lhs, const Mat3& rhs);

Vec2 operator* (const Mat3& pMtx, const Vec2& rhs);

Mat3 Mat3Identity();

Mat3 Mat3Translate(float x, float y);

Mat3 Mat3Scale(float x, float y);

Mat3 Mat3RotRad(float angle);

Mat3 Mat3RotDeg(float angle);

Mat3 Mat3Transpose(const Mat3& pMtx);

// If the matrix inversion fails, the function returns NULL
Mat3 Mat3Inverse(float* determinant, const Mat3& pMtx);