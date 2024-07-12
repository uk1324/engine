#include "SimplePolygon.hpp"

auto simplePolygonSignedArea(View<const Vec2> verts) -> float {
	// Formula for signed area explained here https://gamemath.com/book/geomprims.html - 9.6.2 Area of a Triangle
	// This is called the shoelace formula. This formula and other formulas for calculating the area of a simple polygon are described here. 
	// https://en.wikipedia.org/wiki/Shoelace_formula
	// Determinant formula
	// https://mathworld.wolfram.com/PolygonArea.html
	if (verts.size() < 3) {
		return 0.0f;
	}
	auto twiceTheSignedArea = 0.0f;
	auto previous = verts.size() - 1;
	for (usize i = 1; i < verts.size(); i++) {
		twiceTheSignedArea += (verts[i].x - verts[previous].x) * (verts[i].y + verts[previous].y);
		previous = i;
	}

	return twiceTheSignedArea / 2.0f;
}

auto simplePolygonArea(View<const Vec2> verts) -> float {
	return abs(simplePolygonSignedArea(verts));
}

auto simplePolygonIsClockwise(View<const Vec2> verts) -> bool {
	// If the signed area is negative then the vertices are counter-clockwise.
	return simplePolygonSignedArea(verts) > 0.0f;
}
