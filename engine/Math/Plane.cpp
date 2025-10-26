#include "Plane.hpp"

Plane::Plane(Vec3 n, float d)
	: n(n)
	, d(d) {}

Plane Plane::fromPoints(const Vec3& a, const Vec3& b, const Vec3& c) {
	const auto edge0 = c - a;
	const auto edge1 = b - a;
	const auto normal = cross(edge0, edge1).normalized();
	/*
	The shortest vector from a point to a plane is always perpendicular to the plane.
	If you draw a line segment perpendicular a line in 2D from any point then if you draw any other line to a
	different point on the plane then the line will create a hypotenuse of a triangle.
	The hypotenuse is always longer than the other edges.
	Projecting any point on the line onto the normalized normal will give the distance to the origin.
	*/
	const auto distance = dot(a, normal);
	return Plane(normal, distance);
}

Plane Plane::fromPointAndNormal(Vec3 point, Vec3 normal) {
	return Plane(normal.normalized(), dot(normal, point));
}

float Plane::signedDistance(const Vec3& p) const {
	return dot(p, n) - d;
}

float Plane::distance(const Vec3& p) const {
	return abs(signedDistance(p));
}

bool Plane::isOnPositiveSide(const Vec3& p) const {
	return signedDistance(p) > 0;
}

std::optional<f32> rayPlaneIntersection(Vec3 rayOrigin, Vec3 rayDirection, const Plane& plane) {
	// Assuming vectors are all normalized
	const auto d = dot(plane.n, rayDirection);
	const auto epsilon = 1e-6;

	// Perpendicular
	if (abs(d) < epsilon) {
		return std::nullopt;
	}
	const auto pointOnPlane = plane.n * plane.d;
	const auto t = dot(pointOnPlane - rayOrigin, plane.n) / d;

	// Wrong direction
	if (t < 0.0f) {
		return std::nullopt;
	}
	return t;
}

std::optional<f32> rayPlaneIntersection(const Ray3& ray, const Plane& plane) {
	return rayPlaneIntersection(ray.origin, ray.direction, plane);
}
