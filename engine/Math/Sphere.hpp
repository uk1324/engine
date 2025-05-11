#pragma once

#include "Ray3.hpp"
#include <optional>

struct Sphere {
	Vec3 center;
	f32 radius;
	Sphere(Vec3 center, f32 radius);
	static Sphere thoughPoints(Vec3 a, Vec3 b, Vec3 c, Vec3 d);
};

std::optional<f32> raySphereIntersection(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, f32 sphereRadius);
std::optional<f32> raySphereIntersection(Vec3 rayOrigin, Vec3 rayDirection, const Sphere& sphere);
std::optional<Vec3> raySphereIntersectionPoint(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, f32 sphereRadius);
std::optional<f32> raySphereIntersection(const Ray3& ray, Vec3 sphereCenter, f32 sphereRadius);