#include "Triangle.hpp"

Triangle::Triangle(Vec2 v0, Vec2 v1, Vec2 v2)
	: v{ v0, v1, v2 } {}

static float sign(float x) { 
	return x < 0.0f ? -1.0f : 1.0f;
}

auto Triangle::contains(Vec2 p) const -> bool {
	return triContains(v, p);
}

auto Triangle::containsWithEpsilon(Vec2 p, float epsilon) -> bool {
	auto area0 = det(v[1] - v[0], p - v[0]);
	auto area1 = det(v[2] - v[1], p - v[1]);
	auto area2 = det(v[0] - v[2], p - v[2]);
	return (area0 < epsilon&& area1 < epsilon&& area2 < epsilon)
		|| (area0 > -epsilon && area1 > -epsilon && area2 > -epsilon);
}

auto Triangle::isClockwise() const -> bool {
	return det(v[0] - v[1], v[0] - v[2]) < 0.0f;
}

auto Triangle::area() -> bool {
	return abs(det(v[0] - v[1], v[0] - v[2])) / 2.0f;
}

bool triContains(const Vec2* v, Vec2 p) {
	auto area0 = det(v[1] - v[0], p - v[0]);
	auto area1 = det(v[2] - v[1], p - v[1]);
	auto area2 = det(v[0] - v[2], p - v[2]);
	auto sign = [](float value) -> float { 
		// "<=" So points on the triangle are also return true.
		return value <= 0.0f ? -1.0f : 1.0f; 
	};
	return sign(area0) == sign(area1) && sign(area1) == sign(area2);
}

Vec2 triCentroid(const Vec2* v) {
	return (v[0] + v[1] + v[2]) / 3.0f;
}

Vec3 barycentricCoordinates(const Vec2* v, Vec2 p) {
	const f32 
		x1 = v[0].x, x2 = v[1].x, x3 = v[2].x,
		y1 = v[0].y, y2 = v[1].y, y3 = v[2].y;

	const f32 area = (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);

	// Calculate barycentric weights using 2D cross product
	auto w1 = (p.x - x2) * (y1 - y2) - (x1 - x2) * (p.y - y2);
	auto w2 = (p.x - x3) * (y2 - y3) - (x2 - x3) * (p.y - y3);
	auto w3 = (p.x - x1) * (y3 - y1) - (x3 - x1) * (p.y - y1);
	w1 /= area;
	w2 /= area;
	w3 /= area;

	return Vec3(w2, w3, w1);
}
