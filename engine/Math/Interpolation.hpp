#pragma once

#include <View.hpp>
#include "Vec2.hpp"

template<typename T, typename U>
auto lerp(T a, T b, U t) -> T {
	// This is better than a + (b - a) * t because it isn't as affected by rounding errors as much.
	return a * (1 - t) + b * t;
}

f32 piecewiseLinearSample(View<const Vec2> points, f32 x);