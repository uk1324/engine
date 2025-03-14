#include "Line.hpp"
#include "Mat2.hpp"
#include <cmath>

Line::Line(Vec2 n, float d)
	: n{ n }
	, d{ d } {}

Line::Line(Vec2 a, Vec2 b)
	: n((b - a).rotBy90deg().normalized()) 
	, d(dot(n, b)) {
}

Line Line::fromPointAndNormal(Vec2 point, Vec2 normal) {
	return Line(normal, dot(normal, point));
}

Line Line::fromParametric(Vec2 point, Vec2 tangent) {
	return fromPointAndNormal(point, tangent.rotBy90deg());
}

auto Line::translated(Vec2 v) const -> Line {
	return Line{ n, d + dot(n, v) };
}

auto Line::distanceAlong(Vec2 v) const -> float {
	return det(n, v);
}

auto Line::projectPointOntoLine(Vec2 p) const -> Vec2 {
	return p - n * distance(*this, p);
}

auto Line::intersection(const Line& other) const -> std::optional<Vec2> {
	// Ax = b
	// x = inv(A) * b

	/*
	x * n1.x + y * n1.y = d
	x * n2.x + y * n2.y = d
	*/

	const auto a = Mat2::fromRows(n, other.n);
	if (a.det() == 0.0f) {
		return std::nullopt;
	}

	const Vec2 b{ d, other.d };
	return a.inversed() * b;
}

auto signedDistance(const Line& l, Vec2 p) -> float {
	return l.d - dot(l.n, p);
}

auto distance(const Line& l, Vec2 p) -> float {
	return abs(signedDistance(l, p));
}
