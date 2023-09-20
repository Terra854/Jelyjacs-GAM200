#include <Line.h>
#include <Debug.h>

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