#include "DouglassPecker.hpp"
#include "Line.hpp"

// https://dyn4j.org/2021/06/2021-06-10-simple-polygon-simplification/
// This algorithm recursively takes the endpoints of the polygon and then finds the point most distant from the line (v[first], v[last]). Then if all the points are less than maxDistanceFromLine from the line then it removes them, because they don't add much to the shape. If there is at least one point that is further than maxDistanceFromLine from the line then it splits the line at that point and runs the algorithm on the 2 split parts.
std::vector<Vec2> polygonDouglassPeckerSimplify(View<const Vec2> verts, float maxDistanceFromLine) {
	if (verts.size() <= 2) {
		return std::vector<Vec2>{ verts.begin(), verts.end() };
	}

	const auto& first = verts[0];
	const auto& last = verts[verts.size() - 1];

	auto maxDistance = -std::numeric_limits<float>::infinity();
	i64 maxVertex = 1;
	for (i64 i = 1; i < static_cast<i64>(verts.size()) - 1; i++) {
		const auto d = distance(Line{ first, last }, verts[i]);
		if (d > maxDistance) {
			maxDistance = d;
			maxVertex = i;
		}
	}

	if (maxDistance >= maxDistanceFromLine) {
		// If there is a point on the line (first, last) that is important (is more than maxDistanceFromLine distance from line then split at that point and apply the algorithm on the split parts.
		auto simplified = polygonDouglassPeckerSimplify(constView(std::vector<Vec2>(verts.begin(), verts.begin() + maxVertex + 1)), maxDistanceFromLine);
		const auto otherPart = polygonDouglassPeckerSimplify(constView(std::vector<Vec2>(verts.begin() + maxVertex + 1, verts.begin() + verts.size())), maxDistanceFromLine);
		simplified.insert(simplified.end(), otherPart.begin(), otherPart.end());
		return simplified;
	} else {
		// If all the points between first and last are really close to the line (first, last) then remove all of them.
		return std::vector<Vec2>{ first, last };
	}
}