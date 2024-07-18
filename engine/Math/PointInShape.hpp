#pragma once 

#include <View.hpp>
#include "Vec2.hpp"

// https://en.wikipedia.org/wiki/Linear_equation
// Different representations of lines
// y = mx + n
// m = (y2 - y1) / (x2 - x1)
// y1 = m * x1 + n
// n = y1 - m * x1
// Slope intercept form
// y = m * x + y1 - m * x1
// y - y1 = m * (x - x1)
// y - y1 = (y2 - y1) / (x2 - x1) * (x - x1)
// Even though there is a multiplication by zero it can be verified that it is also gives the correct result for x1 = x2. This is shown below.
// (x2 - x1) * (y - y1) = (y2 - y1) * (x - x1)
// Determinant form. This just means the the line from endpoint 1 to endpoint 2 is colinear with the line from endpoint 1 to the point (x, y).
// (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1) = 0
// y * (x2 - x1) - y1 * (x2 - x1) - (x * (y2 - y1) - x1 * (y2 - y1)) = 0
// y * (x2 - x1) - y1 * (x2 - x1) - x * (y2 - y1) + x1 * (y2 - y1) = 0
// y * (x2 - x1) - y1 * (x2 - x1) - x * (y2 - y1) + x1 * (y2 - y1) = 0
// y * (x2 - x1) - (y1 * x2 - y1 * x1) - x * (y2 - y1) + x1 * y2 - x1 * y1 = 0
// y * (x2 - x1) - y1 * x2 + y1 * x1 - x * (y2 - y1) + x1 * y2 - x1 * y1 = 0
// y * (x2 - x1) - y1 * x2 - x * (y2 - y1) + x1 * y2 = 0
// -x * (y2 - y1) + y * (x2 - x1) + (x1 * y2 - y1 * x2) = 0
// Normal offset form
// x * (y1 - y2) + y * (x2 - x1) + (x1 * y2 - y1 * x2) = 0

// This formula doesn't work when x1 = x2 = h and y1 = y2 = g. There are infinitely many lines going through a single point.
// x * (y1 - y2) + y * (x2 - x1) + (x1 * y2 - y1 * x2) = 0
// x * (g - g) + y * (h - h) + (h * g - g * h) = 0
// 0 = 0

// h = x1 = x2 and y1 != y2
// x * (y1 - y2) + y * (h - h) + (h * y2 - y1 * h) = 0
// x * (y1 - y2) + y * 0 + h * (y2 - y1) = 0
// x * (y1 - y2) - h * (y1 - y2) = 0
// (x - h) * (y1 - y2) = 0
// y1 != y2
// y1 - y2 != 0
// x - h = 0
// x = h

bool isPointInPolygon(View<const Vec2> verts, Vec2 p);
bool isPointInTransformedPolygon(View<const Vec2> verts, Vec2 translation, f32 rotation, Vec2 p);
bool isPointInCircle(Vec2 center, f32 radius, Vec2 point);