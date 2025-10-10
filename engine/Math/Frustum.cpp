#include "Frustum.hpp"

Frustum Frustum::fromMatrix(const Mat4& toNdc) {
	const auto c = corners(toNdc);
	const Frustum result{
		.planes = {
			/*Plane::fromPoints(c[FRONT_BOTTOM_LEFT_CORNER], c[FRONT_BOTTOM_RIGHT_CORNER], c[FRONT_TOP_RIGHT_CORNER]),
			Plane::fromPoints(c[BACK_TOP_RIGHT_CORNER], c[BACK_BOTTOM_RIGHT_CORNER], c[BACK_BOTTOM_LEFT_CORNER]),
			Plane::fromPoints(c[BACK_BOTTOM_LEFT_CORNER], c[FRONT_BOTTOM_LEFT_CORNER], c[FRONT_TOP_LEFT_CORNER]),
			Plane::fromPoints(c[FRONT_TOP_RIGHT_CORNER], c[FRONT_BOTTOM_RIGHT_CORNER], c[BACK_BOTTOM_RIGHT_CORNER]),
			Plane::fromPoints(c[FRONT_TOP_LEFT_CORNER], c[FRONT_TOP_RIGHT_CORNER], c[BACK_TOP_RIGHT_CORNER]),
			Plane::fromPoints(c[BACK_BOTTOM_RIGHT_CORNER], c[FRONT_BOTTOM_RIGHT_CORNER], c[FRONT_BOTTOM_LEFT_CORNER]),*/

			Plane::fromPoints(c[FRONT_BOTTOM_RIGHT_CORNER], c[FRONT_BOTTOM_LEFT_CORNER], c[FRONT_TOP_RIGHT_CORNER]),
			Plane::fromPoints(c[BACK_BOTTOM_RIGHT_CORNER], c[BACK_TOP_RIGHT_CORNER], c[BACK_BOTTOM_LEFT_CORNER]),
			Plane::fromPoints(c[FRONT_BOTTOM_LEFT_CORNER], c[BACK_BOTTOM_LEFT_CORNER], c[FRONT_TOP_LEFT_CORNER]),
			Plane::fromPoints(c[FRONT_BOTTOM_RIGHT_CORNER], c[FRONT_TOP_RIGHT_CORNER], c[BACK_BOTTOM_RIGHT_CORNER]),
			Plane::fromPoints(c[FRONT_TOP_RIGHT_CORNER], c[FRONT_TOP_LEFT_CORNER], c[BACK_TOP_RIGHT_CORNER]),
			Plane::fromPoints(c[FRONT_BOTTOM_RIGHT_CORNER], c[BACK_BOTTOM_RIGHT_CORNER], c[FRONT_BOTTOM_LEFT_CORNER]),
		}
	};
	//const auto t = result.planes[0].signedDistance(c[BACK_BOTTOM_RIGHT_CORNER]);
	//// This should be negative, because it is on the inside of the frustum so it should be on the negative side of the plane.
	//CHECK(t < 0.0f);

	//const auto t0 = result.planes[2].signedDistance(c[FRONT_BOTTOM_RIGHT_CORNER]);
	//const auto t1 = result.planes[2].signedDistance(c[BACK_BOTTOM_RIGHT_CORNER]);
	//const auto t2 = result.planes[2].signedDistance(c[FRONT_TOP_RIGHT_CORNER]);
	//const auto t3 = result.planes[2].signedDistance(c[BACK_TOP_RIGHT_CORNER]);

	//const auto t4 = result.planes[3].signedDistance(c[FRONT_BOTTOM_LEFT_CORNER]);
	//const auto t5 = result.planes[3].signedDistance(c[BACK_BOTTOM_LEFT_CORNER]);
	//const auto t6 = result.planes[3].signedDistance(c[FRONT_TOP_LEFT_CORNER]);
	//const auto t7 = result.planes[3].signedDistance(c[BACK_TOP_LEFT_CORNER]);

	//const auto t8 = result.planes[4].signedDistance(c[FRONT_BOTTOM_LEFT_CORNER]);
	//const auto t9 = result.planes[4].signedDistance(c[BACK_BOTTOM_LEFT_CORNER]);
	//const auto t10 = result.planes[4].signedDistance(c[FRONT_BOTTOM_RIGHT_CORNER]);
	//const auto t11 = result.planes[4].signedDistance(c[FRONT_BOTTOM_RIGHT_CORNER]);

	//const auto t12 = result.planes[5].signedDistance(c[FRONT_TOP_LEFT_CORNER]);
	//const auto t13 = result.planes[5].signedDistance(c[BACK_TOP_LEFT_CORNER]);
	//const auto t14 = result.planes[5].signedDistance(c[FRONT_TOP_RIGHT_CORNER]);
	//const auto t15 = result.planes[5].signedDistance(c[FRONT_TOP_RIGHT_CORNER]);

 	return result;
}

std::array<Vec3, 8> Frustum::corners(const Mat4& toNdc) {
	const auto fromNdcToFrustum = toNdc.inversed();
	auto transform = [&](Vec3 v) -> Vec3 {
		const auto a = Vec4(v, 1.0f) * fromNdcToFrustum;
		return a.xyz() / a.w;
	};
	const std::array<Vec3, 8> corners = {
		transform(Vec3(1, 1, -1)),
		transform(Vec3(1, -1, -1)),
		transform(Vec3(-1, 1, -1)),
		transform(Vec3(-1, -1, -1)),
		transform(Vec3(1, 1, 1)),
		transform(Vec3(1, -1, 1)),
		transform(Vec3(-1, 1, 1)),
		transform(Vec3(-1, -1, 1)),

		/*transform(Vec3(1, 1, 1)),
		transform(Vec3(1, -1, 1)),
		transform(Vec3(-1, 1, 1)),
		transform(Vec3(-1, -1, 1)),
		transform(Vec3(1, 1, -1)),
		transform(Vec3(1, -1, -1)),
		transform(Vec3(-1, 1, -1)),
		transform(Vec3(-1, -1, -1)),*/
	};
	return corners;
}

bool Frustum::intersects(const Aabb3& aabb) const {

	// This seems broken. 
	ASSERT_NOT_REACHED();
	//auto anyPointsOfAabbOnNegativeSide = [&](const Plane& plane) -> bool {
	//	const auto size = aabb.size();
	//	// TODO: Why is this adding to min instead of using the coordinates of max.
	//	// Why recompute the corner points for each plane
	//	return plane.isOnPositiveSide(aabb.min)
	//		|| plane.isOnPositiveSide(aabb.max)
	//		|| plane.isOnPositiveSide(aabb.min + Vec3(size.x, 0, 0))
	//		|| plane.isOnPositiveSide(aabb.min + Vec3(0, size.y, 0))
	//		|| plane.isOnPositiveSide(aabb.min + Vec3(0, 0, size.z))
	//		|| plane.isOnPositiveSide(aabb.min + Vec3(0, size.y, size.z))
	//		|| plane.isOnPositiveSide(aabb.min + Vec3(size.x, 0, size.z))
	//		|| plane.isOnPositiveSide(aabb.min + Vec3(size.x, size.y, 0));
	//	};

	//// Why continue looping when we alredy know it's false?
	//bool inside = true;
	//for (i32 i = 0; i < PLANE_COUNT; i++) {
	//	inside &= anyPointsOfAabbOnNegativeSide(planes[i]);
	//}
	//return inside;
}

bool Frustum::intersects(const Box3& b) const {
	//auto anyPointsOfAabbOnNegativeSide = [&](const Plane& plane) -> bool {
	//	//const auto size = aabb.size();
	//	// TODO: Why is this adding to min instead of using the coordinates of max.
	//	// Why recompute the corner points for each plane
	//	const auto v = b.vertices();

	//	return !plane.isOnPositiveSide(v[0])
	//		|| !plane.isOnPositiveSide(v[1])
	//		|| !plane.isOnPositiveSide(v[2])
	//		|| !plane.isOnPositiveSide(v[3])
	//		|| !plane.isOnPositiveSide(v[4])
	//		|| !plane.isOnPositiveSide(v[5])
	//		|| !plane.isOnPositiveSide(v[6])
	//		|| !plane.isOnPositiveSide(v[7]);
	//	};

	//// Why continue looping when we alredy know it's false?
	//bool inside = true;
	//for (i32 i = 0; i < PLANE_COUNT; i++) {
	//	inside &= anyPointsOfAabbOnNegativeSide(planes[i]);
	//}
	//return inside;

	const auto vertices = b.vertices();
	auto allPointsOnTheOusideSide = [&](const Plane& plane) -> bool {
		for (const auto& v : vertices) {
			if (isPointOnInsideSide(v, plane)) {
				return false;
			}
		}
		return true;
	};

	for (i32 i = 0; i < PLANE_COUNT; i++) {
		// If all points on the outside side of a single plane the that plane separates the box from the inside of the frustum.
		if (allPointsOnTheOusideSide(planes[i])) {
			return false;
		}
	}
	return true;
}

bool Frustum::intersectsTriangle(Vec3 v0, Vec3 v1, Vec3 v2) const {
	const Vec3 vertices[] { v0, v1, v2 };
	auto allPointsOnTheOusideSide = [&](const Plane& plane) -> bool {
		for (const auto& v : vertices) {
			if (isPointOnInsideSide(v, plane)) {
				return false;
			}
		}
		return true;
	};

	for (i32 i = 0; i < PLANE_COUNT; i++) {
		// If all points on the outside side of a single plane the that plane separates the box from the inside of the frustum.
		if (allPointsOnTheOusideSide(planes[i])) {
			return false;
		}
	}
	return true;
}

bool Frustum::isPointOnInsideSide(Vec3 v, const Plane& frustumPlane) const {
	return !frustumPlane.isOnPositiveSide(v);
}
