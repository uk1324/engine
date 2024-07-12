#include "PointInShape.hpp"

// https://observablehq.com/@tmcw/understanding-point-in-polygon
// https://wrfranklin.org/Research/Short_Notes/pnpoly.html
// IMPLEMENTING SIMULATION OF SIMPLICITY FOR DEGENERACIES https://wrfranklin.org/p/234-implementing-sos-2022.pdf. Has some citations about triangulation.
auto isPointInPolygon(View<const Vec2> verts, Vec2 p) -> bool {
	// Jordan curve theorem.
	bool crossedOddTimes = false;
	usize previous = verts.size() - 1;
	for (usize i = 0; i < verts.size(); i++) {
		const auto& a = verts[previous];
		const auto& b = verts[i];

		// Because the algorithms sends a horizontal ray the edges which it doesn't cross can be ignored. They actually have to be ignored, because the algorithm does ray vs line intersection and not ray vs line segment intersection.
		const auto pointYIsBetweenEdgeEndpointsY = (b.y > p.y) != (a.y > p.y); // The y's aren't both above or both below. One is above and one is below.

		// Intersection of the line going through a and b with the line y = p.y.
		// Determinant representation
		// (x2 - x1) * (y - y1) = (y2 - y1) * (x - x1)
		// (x2 - x1) * (p.y - y1) = x * (y2 - y1) - x1 * (y2 - y1)
		// x * (y2 - y1) = (x2 - x1) * (p.y - y1) + x1 * (y2 - y1)
		// x * (y2 - y1) = (x2 - x1) * (p.y - y1) + x1 * (y2 - y1)
		// x = (x2 - x1) * (p.y - y1) / (y2 - y1) + x1
		// x = -(x1 - x2) * (p.y - y1) / -(y1 - y2) + x1
		// This version is apparently better, because it handles cases like a line going through a vertex correctly.
		// x = (p.y * (x1 - x2) - y1 * (x1 - x2)) / (y1 - y2) + x1

		// (x2 - x1) * (y - y1) = (y2 - y1) * (x - x1)
		// Swapping x1 and x2 and y1 and y2 apparently makes it so the cases like the ray passing through an edge is handled correctly.
		// The solution without the swap is x = (x1 - x2) * (y - y1) / (y1 - y2) + x1. From what I checked this does change the result slightly.
		// (x1 - x2) * (y - y2) = (y1 - y2) * (x - x2)
		// (x1 - x2) * (y - y2) = x * (y1 - y2) - x2 * (y1 - y2)
		// x * (y1 - y2) = (x1 - x2) * (y - y2) + x2 * (y1 - y2)
		// pointYIsBetweenEdgeEndpointsY ensures that y2 != y1 => y2 - y1 != 0, because it can't be between if the ys are the same.
		// x = (x1 - x2) * (y - y2) / (y1 - y2) + x2
		// y = p.y
		// x = (x1 - x2) * (p.y - y2) / (y1 - y2) + x2

		// TODO: This can divide by zero, but when it does the result isn't used.
		const auto lineIntersectionX = (a.x - b.x) * (p.y - b.y) / (a.y - b.y) + b.x;

		// The ray goes from the right side to the left side so any intersections to the left of the point are not counted.
		const auto doesRayIntersect = p.x < lineIntersectionX;
		if (pointYIsBetweenEdgeEndpointsY && doesRayIntersect)
			crossedOddTimes = !crossedOddTimes;

		previous = i;
	}
	return crossedOddTimes;
}

bool isPointInCircle(Vec2 center, f32 radius, Vec2 point) {
	return center.distanceSquaredTo(point) <= radius * radius;
}