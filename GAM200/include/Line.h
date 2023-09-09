#pragma once

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

void Line::RecalculateLineNormal() {
	normal = Mat3{ 0.f, 1.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 1.f } *(pt1 - pt0) / sqrt(normal.x * normal.x + normal.y * normal.y);
}

Vec2 Line::Pt0() const {
	return pt0;
}

Vec2 Line::Pt1() const {
	return pt1;
}

Vec2 Line::Normal() const {
	return normal;
}

void Line::SetPt0(Vec2 v) {
	pt0 = v;
	RecalculateLineNormal();
}

void Line::SetPt1(Vec2 v) {
	pt1 = v;
	RecalculateLineNormal();
}