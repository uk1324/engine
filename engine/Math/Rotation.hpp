#pragma once

#include "Vec2.hpp"

struct Rotation {
	Rotation(float cos, float sin);
	Rotation(float angle);

	Rotation inversed() const;
	//Rotation rotated90DegCounterclockwise();

	float cos, sin;
};

Vec2 operator*(Rotation rotation, Vec2 v);
void operator*=(Vec2& v, Rotation rotation);
