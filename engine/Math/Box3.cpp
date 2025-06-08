#include "Box3.hpp"
#include <engine/Math/Angles.hpp>

Box3 Box3::containingCircleTube(Vec3 circleCenter, Vec3 circleStartRelativeToCenter, Vec3 circleVelocity, f32 tubeRadius) {
	Box3 box;
	box.center = circleCenter;
	box.directions[0] = circleStartRelativeToCenter.normalized();
	box.directions[1] = circleVelocity.normalized();
	box.directions[2] = cross(box.directions[0], box.directions[1]).normalized();
	const auto radius = circleStartRelativeToCenter.length();
	box.halfSize = Vec3(radius + tubeRadius, radius + tubeRadius, tubeRadius);
	return box;
}

Box3 Box3::containingCircleArcTube(Vec3 circleCenter, Vec3 circleStartRelativeToCenter, Vec3 circleVelocity, f32 angle, f32 tubeRadius) {
	Box3 box;
	auto sample = [&](f32 a) -> Vec3 {
		return circleCenter + circleStartRelativeToCenter * cos(a) + circleVelocity * sin(a);
	};
	const auto& e0 = circleCenter + circleStartRelativeToCenter;
	const auto& e1 = sample(angle);
	const auto circleMid = sample(angle / 2.0f);
	const auto mid = (e0 + e1) / 2.0f;
	const auto midAxis = mid - circleMid;
	const auto midAxisLength = midAxis.length();
	box.center = (circleMid + mid) / 2.0f;
	const auto circleRadius = circleStartRelativeToCenter.length();
	box.halfSize = Vec3(tubeRadius, midAxisLength / 2.0f + tubeRadius, circleRadius + tubeRadius);
	if (angle < PI<f32>) {
		box.halfSize.z = e0.distanceTo(e1) / 2.0f + tubeRadius;
	}
	box.directions[0] = cross(circleStartRelativeToCenter, circleVelocity).normalized();
	box.directions[1] = midAxis / midAxisLength;
	box.directions[2] = cross(box.directions[0], box.directions[1]).normalized();
	return box;
}

Box3 Box3::containingRoundCappedCyllinder(Vec3 e0, Vec3 e1, f32 radius) {
	Box3 box;
	box.center = (e0 + e1) / 2.0f;
	auto v = e1 - e0;
	const auto length = v.length();
	v /= length;
	box.halfSize = Vec3(length / 2.0f + radius, radius, radius);
	box.directions[0] = v;
	box.directions[1] = anyPerpendicularVector(v);
	box.directions[2] = cross(box.directions[0], box.directions[1]);
	return box;
}

std::array<Vec3, 8> Box3::vertices() const {
	const auto& c = center;
	const auto& s = halfSize;
	const auto& d = directions;
	return {
		c - s.x * d[0] - s.y * d[1] - s.z * d[2],
		c - s.x * d[0] - s.y * d[1] + s.z * d[2],
		c - s.x * d[0] + s.y * d[1] - s.z * d[2],
		c - s.x * d[0] + s.y * d[1] + s.z * d[2],
		c + s.x * d[0] - s.y * d[1] - s.z * d[2],
		c + s.x * d[0] - s.y * d[1] + s.z * d[2],
		c + s.x * d[0] + s.y * d[1] - s.z * d[2],
		c + s.x * d[0] + s.y * d[1] + s.z * d[2]
	};
}