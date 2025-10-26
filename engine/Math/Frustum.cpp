#include "Frustum.hpp"

Frustum Frustum::fromMatrix(const Mat4& toNdc) {
	const auto c = calculateCorners(toNdc);
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
		},
		.corners = c,
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

std::array<Vec3, 8> Frustum::calculateCorners(const Mat4& toNdc) {
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

// https://iquilezles.org/articles/frustumcorrect/

bool Frustum::intersectsTriangleInexact(Vec3 v0, Vec3 v1, Vec3 v2) const {
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

bool Frustum::intersectsTriangleInexact1(Vec3 v0, Vec3 v1, Vec3 v2) const {
	const auto trianglePlane = Plane::fromPoints(v0, v1, v2);
	auto side = [&](Vec3 point) {
		return trianglePlane.signedDistance(point) > 0.0f;
	};
	bool cornerSide0 = side(corners[0]);
	for (i32 i = 1; i < CORNER_COUNT; i++) {
		if (cornerSide0 != side(corners[i])) {
			return true;
		}
	}

	Vec3 vertices[]{ v0, v1, v2 };
	i32 previousVertex = 2;
	for (i32 vI = 0; vI < 3; vI++) {
		const auto edge = vertices[previousVertex] - vertices[vI];
		const auto edgeNormal = cross(trianglePlane.n, edge).normalized();
		const auto edgePlane = Plane::fromPointAndNormal(vertices[vI], edgeNormal);

		auto side = [&](Vec3 point) {
			return edgePlane.signedDistance(point) > 0.0f;
		};

		{
			bool cornerSide0 = side(corners[0]);
			for (i32 i = 1; i < CORNER_COUNT; i++) {
				if (cornerSide0 != side(corners[i])) {
					return true;
				}
			}
		}


		previousVertex = vI;
	}
	// Work's because the triangle plane separates the triangle from the frustum.
	// Won't work for example if the triangle plane is parallel to the front and back plane and is between them. Then there will be points on both sides of the triangle plane. It's easy to see this remains true if the triangle is rotated a bit and there are many other cases.
	return false;
}

bool Frustum::intersectsSphere(Vec3 center, f32 radius) const {
	// https://stackoverflow.com/questions/37512308/choice-of-sphere-frustum-overlap-test
	// Doesn't discard all sphere
	for (i32 i = 0; i < PLANE_COUNT; i++) {
		// If all points on the outside side of a single plane the that plane separates the box from the inside of the frustum.
		if (!isSphereOnInsideSide(center, radius, planes[i])) {
			return false;
		}
	}
	return true;
}

bool Frustum::isPointOnInsideSide(Vec3 v, const Plane& frustumPlane) const {
	return !frustumPlane.isOnPositiveSide(v);
}


bool Frustum::isSphereOnInsideSide(Vec3 center, f32 radius, const Plane& frustumPlane) const {
	// A more precise name might is any part of the sphere on the inside side.
	return frustumPlane.signedDistance(center) < radius;
}