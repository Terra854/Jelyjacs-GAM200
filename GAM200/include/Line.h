#pragma once
#include <Debug.h>

#include "Vec2.h"
#include "Mat3.h"

#include <cmath>

class Line
{
private:
    Vec2 pt0, pt1, normal;
	void RecalculateLineNormal();
public:
	Line(){}
	Line(Vec2 _pt0, Vec2 _pt1) : pt0(_pt0), pt1(_pt1) {
		RecalculateLineNormal();
	}

	Line& operator=(const Line& rhs) = default;
	Line(const Line& rhs) = default;

	Vec2 Pt0() const;
	Vec2 Pt1() const;
	Vec2 Normal() const;

	void SetPt0(Vec2 v);
	void SetPt1(Vec2 v);
};