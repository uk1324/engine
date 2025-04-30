#include "Circle.hpp"

Circle::Circle(Vec2 center, f32 radius)
	: center(center)
	, radius(radius) {}

// https://www.johndcook.com/blog/2023/06/18/circle-through-three-points/
Circle Circle::thoughPoints(Vec2 p0, Vec2 p1, Vec2 p2) {
	/*const auto x1 = p0.x, y1 = p0.y, x2 = p1.x, y2 = p1.y, x3 = p2.x, y3 = p2.y;
	const auto s1 = pow(x1, 2.0f) + pow(y1, 2.0f);
	const auto s2 = pow(x2, 2.0f) + pow(y2, 2.0f);
	const auto s3 = pow(x3, 2.0f) + pow(y3, 2.0f);
	const auto m11 = x1 * y2 + x2 * y3 + x3 * y1 - (x2 * y1 + x3 * y2 + x1 * y3);
	const auto m12 = s1 * y2 + s2 * y3 + s3 * y1 - (s2 * y1 + s3 * y2 + s1 * y3);
	const auto m13 = s1 * x2 + s2 * x3 + s3 * x1 - (s2 * x1 + s3 * x2 + s1 * x3);
	const auto x0 = 0.5f * m12 / m11;
	const auto y0 = -0.5f * m13 / m11;
	const auto r0 = sqrt(pow((x1 - x0), 2.0f) + pow((y1 - y0), 2.0f));*/
	using CalculationType = f64;
	const CalculationType x1 = p0.x, y1 = p0.y, x2 = p1.x, y2 = p1.y, x3 = p2.x, y3 = p2.y;
	const CalculationType s1 = pow(x1, CalculationType(2.0)) + pow(y1, CalculationType(2.0));
	const CalculationType s2 = pow(x2, CalculationType(2.0)) + pow(y2, CalculationType(2.0));
	const CalculationType s3 = pow(x3, CalculationType(2.0)) + pow(y3, CalculationType(2.0));
	const CalculationType m11 = x1 * y2 + x2 * y3 + x3 * y1 - (x2 * y1 + x3 * y2 + x1 * y3);
	const CalculationType m12 = s1 * y2 + s2 * y3 + s3 * y1 - (s2 * y1 + s3 * y2 + s1 * y3);
	const CalculationType m13 = s1 * x2 + s2 * x3 + s3 * x1 - (s2 * x1 + s3 * x2 + s1 * x3);
	const CalculationType x0 = CalculationType(0.5) * m12 / m11;
	const CalculationType y0 = -CalculationType(0.5) * m13 / m11;
	const CalculationType r0 = sqrt(pow((x1 - x0), CalculationType(2.0)) + pow((y1 - y0), CalculationType(2.0)));
	return Circle(Vec2(f32(x0), f32(y0)), f32(r0));
}
