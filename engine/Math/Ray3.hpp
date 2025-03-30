#pragma once

#include <engine/Math/Vec3.hpp>

struct Ray3 {
	Ray3(Vec3 origin, Vec3 direction);
	Vec3 origin;
	Vec3 direction;

	Vec3 at(f32 t) const;
};

Vec3 rayAt(f32 t, Vec3 rayOrigin, Vec3 rayDirection);