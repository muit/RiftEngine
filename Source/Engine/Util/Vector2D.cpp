// Copyright 2015-2019 Piperift - All rights reserved

#include "Vector2D.h"
#include <math.h>


const Vector2D Vector2D::Zero{ 0, 0 };
const Vector2D Vector2D::One { 1, 1 };

constexpr Vector2D Vector2D::FromAngle(float angle, float intensity) {
	angle = DegToRad(angle);
	return {
		intensity * cos(angle),
		intensity * -sin(angle)
	};
}

constexpr Vector2D operator+(Vector2D lhs, const Vector2D rhs) {
	lhs += rhs;
	return lhs;
}

constexpr Vector2D operator-(Vector2D lhs, const Vector2D rhs) {
	lhs -= rhs;
	return lhs;
}

constexpr Vector2D operator*(Vector2D lhs, const Vector2D rhs) {
	lhs *= rhs;
	return lhs;
}

constexpr Vector2D operator*(Vector2D lhs, const float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

constexpr Vector2D operator/(Vector2D lhs, const Vector2D rhs) {
	lhs /= rhs;
	return lhs;
}


ostream & operator<<(ostream &os, const Vector2D& p)
{
	os << '(' << p.x << ", " << p.y << ')';
	return os;
}
