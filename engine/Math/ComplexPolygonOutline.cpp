#include "ComplexPolygonOutline.hpp"
#include "LineSegment.hpp"

std::optional<const std::vector<Vec2>&> complexPolygonOutline(View<const Vec2> vertices, std::vector<Vec2>& result) {
	result.clear();
	if (vertices.size() <= 3) {
		result.insert(result.begin(), vertices.begin(), vertices.end());
		return result;
	}

	const auto leftmostVertex = std::min_element(vertices.begin(), vertices.end(), [](Vec2 a, Vec2 b) { return a.x < b.x; }) - vertices.begin();
	auto getIndex = [&](int i) -> int {
		if (i < 0) {
			i = int(vertices.size() - 1);
		}
		if (i >= vertices.size()) {
			i = 0;
		}
		return i;
	};

	auto get = [&](int i) -> Vec2 {
		return vertices[getIndex(i)];
	};

	// The leftmost index always lies on the shape's boundary. Any <direction>most point would do.
	const int startIndex = int(leftmostVertex);
	const auto a = get(startIndex - 1);
	const auto b = get(startIndex);
	const auto c = get(startIndex + 1);
	const auto mid = (a + c) / 2.0f;

	int direction;
	if (signedDistance(Line{ b, mid }, a) < 0.0f) {
		direction = 1;
	} else {
		direction = -1;
	}

	auto nextIndex = getIndex(startIndex + direction);
	Vec2 current = vertices[startIndex];
	std::optional<int> ignored0 = startIndex;
	int ignored01 = nextIndex;
	std::optional<int> ignored1;
	int ignored11 = 0;
	result.push_back(current);

	i64 iterationCount = 0;
	// upper bound on number of vertices needed to be traversed.
	const auto maxIterations = 
		vertices.size() * (vertices.size() - 1) / 2 + // maximum number intersections of 'number of vertices' lines
		vertices.size(); // 'number of vertices'

	while (true) {
		if (iterationCount >= maxIterations) {
			return std::nullopt;
		}
		iterationCount++;

		const auto line = LineSegment{ current, vertices[nextIndex] };
		const auto sizeBefore = result.size();

		std::optional<Vec2> closestIntersection;
		float closestIntersectionDistance = std::numeric_limits<float>::infinity();
		int closestIntersectionLineStartIndex = 0;
		int closestIntersectionLineEndIndex = 0;

		int previousI = int(vertices.size()) - 1;
		for (int i = 0; i < vertices.size(); previousI = i, i++) {
			// Skip lines that share an endpoint with the ray.
			if (ignored0.has_value()
				&& (i == ignored0 || previousI == ignored01 || i == ignored01 || previousI == ignored0)) {
				continue;
			}

			if (ignored1.has_value()
				&& ((i == ignored1 && previousI == ignored11) || (i == ignored11 && previousI == ignored1))) {
				continue;
			}
			const auto intersection = line.intersection(LineSegment{ vertices[i], vertices[previousI] });
			if (!intersection.has_value()) {
				continue;
			}

			const auto d = distance(*intersection, current);
			if (d < closestIntersectionDistance) {
				closestIntersectionDistance = d;
				closestIntersection = *intersection;
				closestIntersectionLineStartIndex = i;
				closestIntersectionLineEndIndex = previousI;
			}
		}

		if (closestIntersection.has_value()) {
			result.push_back(*closestIntersection);

			current = *closestIntersection;
			ignored1 = ignored0;
			ignored11 = ignored01;

			if (signedDistance(line.line, vertices[closestIntersectionLineStartIndex]) > 0.0f) {
				direction = 1;
				nextIndex = getIndex(closestIntersectionLineStartIndex);
			} else {
				direction = -1;
				nextIndex = getIndex(closestIntersectionLineEndIndex);
			}
			ignored0 = nextIndex;
			ignored01 = getIndex(nextIndex - direction);
		} else {
			if (nextIndex == startIndex) {
				break;
			}

			result.push_back(vertices[nextIndex]);
			ignored0 = nextIndex;

			current = vertices[nextIndex];
			nextIndex = getIndex(nextIndex + direction);

			ignored01 = nextIndex;
			ignored1 = std::nullopt;
		}


		if (result.size() == sizeBefore) {
			break;
		}
	}
	return result;
}