#pragma once

#include "Plane.hpp"
#include "Mat4.hpp"
#include "Box3.hpp"
#include "Aabb3.hpp"
#include <array>

struct Frustum {
	enum Corner {
		BACK_TOP_RIGHT_CORNER,
		BACK_BOTTOM_RIGHT_CORNER,
		BACK_TOP_LEFT_CORNER,
		BACK_BOTTOM_LEFT_CORNER,
		FRONT_TOP_RIGHT_CORNER,
		FRONT_BOTTOM_RIGHT_CORNER,
		FRONT_TOP_LEFT_CORNER,
		FRONT_BOTTOM_LEFT_CORNER,
		CORNER_COUNT,
	};

	enum PlaneType {
		FRONT_PLANE,
		BACK_PLANE,
		LEFT_PLANE,
		RIGHT_PLANE,
		UP_PLANE,
		DOWN_PLANE,
		PLANE_COUNT,
	};

	static Frustum fromMatrix(const Mat4& toNdc);
	static std::array<Vec3, CORNER_COUNT> calculateCorners(const Mat4& toNdc);

	// Being inside also counts as intersecting.
	
	// These tests also give false positives. This is done to make the checking faster.
	bool intersects(const Aabb3& aabb) const;
	bool intersects(const Box3& b) const;
	bool intersectsTriangleInexact(Vec3 v0, Vec3 v1, Vec3 v2) const;
	bool intersectsTriangleInexact1(Vec3 v0, Vec3 v1, Vec3 v2) const;
	bool intersectsSphere(Vec3 center, f32 radius) const;

	// Plane normals point outside. That is the positive side of the plane is the outside.
	// So the positive side is the outside side and the negative side is the inside side.
	Plane planes[PLANE_COUNT];
	std::array<Vec3, CORNER_COUNT> corners;
	bool isPointOnInsideSide(Vec3 v, const Plane& frustumPlane) const;
	bool isSphereOnInsideSide(Vec3 center, f32 radius, const Plane& frustumPlane) const;
};