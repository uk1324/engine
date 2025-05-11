#pragma once

#include "Vec3.hpp"
#include "Ray3.hpp"
#include <optional>

struct Plane {
	// Assumes that n is normalized.
	Plane(Vec3 n, float d);
	static Plane fromPoints(const Vec3& a, const Vec3& b, const Vec3& c);

	float signedDistance(const Vec3& p) const;
	float distance(const Vec3& p) const;
	// The positive is the one that the normal points to. (TODO: Check)
	bool isOnPositiveSide(const Vec3& p) const;

	// dot(n, p) = d
	Vec3 n; // Unit length
	float d;
};

std::optional<f32> rayPlaneIntersection(Vec3 rayOrigin, Vec3 rayDirection, const Plane& plane);
std::optional<f32> rayPlaneIntersection(const Ray3& ray, const Plane& plane);