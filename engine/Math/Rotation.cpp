#include "Rotation.hpp"
#include <math.h>

Rotation::Rotation(float cos, float sin)
	: sin{ sin }
	, cos{ cos } {}

Rotation::Rotation(float angle)
	: sin{ ::sin(angle) }
	, cos{ ::cos(angle) } {}

Rotation Rotation::inversed() const {
	// conjugate
	return Rotation{ cos, -sin };
}

//Rotation Rotation::rotated90DegCounterclockwise() {
//	return Rotation{ -sin, cos };
//}

Vec2 operator*(Rotation rot, Vec2 v) {
	return v.x * Vec2{ rot.cos, rot.sin } + v.y * Vec2{ -rot.sin, rot.cos };
}

void operator*=(Vec2& v, Rotation rotation) {
	v = rotation * v;
}
