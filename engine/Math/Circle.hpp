#pragma once

#include <engine/Math/Vec2.hpp>

struct Circle {
	Circle(Vec2 center, f32 radius);

	static Circle thoughPoints(Vec2 p0, Vec2 p1, Vec2 p2);

	Vec2 center;
	f32 radius;
};
