/* !
@file	Line.cpp
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains the definitions of the member functions in the Line class
*//*__________________________________________________________________________*/
#include "Line.h"
#include <Debug.h>

/******************************************************************************
	RecalculateLineNormal
	- Recalculates the normal of the line
	- Need to call whenever pt0 or pt1 is changed
*******************************************************************************/
void Line::RecalculateLineNormal() {
	normal = Mat3{ 0.f, 1.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 1.f } *(pt1 - pt0);
	normal /= sqrt(normal.x * normal.x + normal.y * normal.y);
}

/******************************************************************************
	Pt0
	- Gets the coordinates of pt0
*******************************************************************************/
Vec2 Line::Pt0() const {
	return pt0;
}

/******************************************************************************
	Pt1
	- Gets the coordinates of pt1
*******************************************************************************/
Vec2 Line::Pt1() const {
	return pt1;
}

/******************************************************************************
	Normal
	- Gets the normal vector
*******************************************************************************/
Vec2 Line::Normal() const {
	return normal;
}

/******************************************************************************
	SetPt0
	- Sets the coordinates of pt0

	@param	v - New pt0 coordinates
*******************************************************************************/
void Line::SetPt0(Vec2 v) {
	pt0 = v;
	RecalculateLineNormal();
}

/******************************************************************************
	SetPt1
	- Sets the coordinates of pt1

	@param	v - New pt1 coordinates
*******************************************************************************/
void Line::SetPt1(Vec2 v) {
	pt1 = v;
	RecalculateLineNormal();
}