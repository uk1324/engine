#include "Interpolation.hpp"

f32 piecewiseLinearSample(View<const Vec2> points, f32 x) {
	if (points.size() == 0.0f) {
		return 0.0f;
	}

	if (x < points[0].x) {
		return points[0].y;
	}

	for (i32 i = 0; i < i32(points.size()) - 1; i++) {
		const auto& next = points[i + 1];
		if (x <= next.x) {
			const auto& previous = points[i];
			return lerp(previous.y, next.y, (x - previous.x) / (next.x - previous.x));
		}
	}

	return points.fromBack(-1).y;
}
