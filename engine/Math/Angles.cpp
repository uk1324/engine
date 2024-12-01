#include "Angles.hpp"
#include <math.h>

f32 angleToRangeZeroTau(f32 a) {
	a -= floorf(a / TAU<f32>) * TAU<f32>;
	return a;
}

bool AngleRange::isInRange(f32 angle) const {
	/*
	min = a0 + m tau
	max = a1 + m tau
	min and max both have the same multiple of tau added because they are a range. That is max is measured with respect to min.
	angle = a + n tau

	subtracting floor(p / TAU<f32>) * TAU<f32> brings the angles into the same range
	*/
	angle -= floorf(angle / TAU<f32>) * TAU<f32>;
	{
		const auto offset = floorf(min / TAU<f32>) * TAU<f32>;
		const auto mn = min - offset;
		const auto mx = max - offset;
		if (angle >= mn && angle <= mx) {
			return true;
		}
	}
	{
		const auto offset = floorf(max / TAU<f32>) * TAU<f32>;
		const auto mn = min - offset;
		const auto mx = max - offset;
		return angle >= mn && angle <= mx;
	}
}