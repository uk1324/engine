#pragma once
#include <engine/Math/Vec3.hpp>
#include <array>

struct Box3 {
	static Box3 containingCircleTube(Vec3 circleCenter, Vec3 circleStartRelativeToCenter, Vec3 circleVelocity, f32 tubeRadius);
	static Box3 containingCircleArcTube(Vec3 circleCenter, Vec3 circleStartRelativeToCenter, Vec3 circleVelocity, f32 angle, f32 tubeRadius);
	static Box3 containingRoundCappedCyllinder(Vec3 e0, Vec3 e1, f32 radius);

	std::array<Vec3, 8> vertices() const;

	Vec3 center;
	Vec3 halfSize;
	Vec3 directions[3];
};