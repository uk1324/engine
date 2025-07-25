#pragma once

#include "Vec2.hpp"
#include "Vec3.hpp"

struct Triangle {
	Triangle(Vec2 v0, Vec2 v1, Vec2 v2);
	Vec2 v[3];
	/*auto counterClockwiseContains(Vec2 p) const -> bool;
	auto clockwiseContains(Vec2 p) const -> bool;*/
	auto contains(Vec2 p) const -> bool;
	auto containsWithEpsilon(Vec2 p, float epsilon) -> bool;
	auto isClockwise() const -> bool;
	auto area() -> bool;
};

bool triContains(const Vec2* v, Vec2 p);
Vec2 triCentroid(const Vec2* v);
Vec3 barycentricCoordinates(const Vec2* v, Vec2 p);
template<typename T>
Vec3 barycentricCoordinatesInterpolate(Vec3 p, const T* v);

template<typename T>
Vec3 barycentricCoordinatesInterpolate(Vec3 p, const T* v) {
	return p.x * v[0] + p.y * v[1] + p.z * v[2];
}
