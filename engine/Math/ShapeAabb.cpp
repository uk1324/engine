#include "ShapeAabb.hpp"
#include "Rotation.hpp"

Aabb circleAabb(Vec2 center, f32 radius) {
	return Aabb(center - Vec2(radius), center + Vec2(radius));
}

Aabb transformedPolygonAabb(View<const Vec2> vertices, Vec2 translation, f32 rotation) {
	Vec2 min{ std::numeric_limits<float>::infinity() }, max{ -std::numeric_limits<float>::infinity() };

	const auto rotationTransformation = Rotation(rotation);
	for (auto point : vertices) {
		point *= rotationTransformation;
		point += translation;
		min = min.min(point);
		max = max.max(point);
	}

	return Aabb{ min, max };
}
