#pragma once

#include <Types.hpp>
#include "Constants.hpp"

template<typename T>
auto degToRad(T s) -> T {
	return s * (TAU<T> / 360);
}

template<typename T>
auto radToDeg(T s) -> T {
	return s * (360 / TAU<T>);
}

// Useful for pereventing precision issues.
template<typename T>
T normalizeAngleZeroToTau(T angle);

template<typename T>
T normalizeAngleZeroToTau(T angle) {
	// not sure what is the best way to do this.
	angle = fmod(angle, TAU<float>);
	if (angle < 0)
		angle += TAU<float>;
	return angle;
}

f32 angleToRangeZeroTau(f32 a);

struct AngleRange {
	f32 min, max;

	bool isInRange(f32 angle) const;
};