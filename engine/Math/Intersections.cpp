#include "Intersections.hpp"
#include <algorithm>

bool sphereIntersectsBox(const Sphere& sphere, const Box3& box) {
	Vec3 spherePos = sphere.center;
	spherePos -= box.center;
	spherePos = Vec3(
		dot(spherePos, box.directions[0]),
		dot(spherePos, box.directions[1]),
		dot(spherePos, box.directions[2])
	);
	if (spherePos.x < -box.halfSize.x && spherePos.x < box.halfSize.x && 
		spherePos.y < -box.halfSize.y && spherePos.y < box.halfSize.y &&
		spherePos.z < -box.halfSize.z && spherePos.z < box.halfSize.z) {
		return true;
	}

	Vec3 closestPointOnBox(
		std::clamp(spherePos.x, -box.halfSize.x, box.halfSize.x),
		std::clamp(spherePos.y, -box.halfSize.y, box.halfSize.y),
		std::clamp(spherePos.z, -box.halfSize.z, box.halfSize.z)
	);
	if (closestPointOnBox.distanceSquaredTo(spherePos) < sphere.radius * sphere.radius) {
		return true;
	}
	return false;
}