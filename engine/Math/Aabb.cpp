#include "Aabb.hpp"

Aabb::Aabb(Vec2 min, Vec2 max)
	: min{ min }
	, max{ max } {}

auto Aabb::fromPosSize(Vec2 pos, Vec2 size) -> Aabb {
	const auto halfSize = size / 2.0f;
	return Aabb{ pos - halfSize, pos + halfSize };
}

auto Aabb::fromCorners(Vec2 a, Vec2 b) -> Aabb {
	Vec2 min(0.0f), max(0.0f);
	if (a.x < b.x) {
		min.x = a.x;
		max.x = b.x;
	} else {
		min.x = b.x;
		max.x = a.x;
	}

	if (a.y < b.y) {
		min.y = a.y;
		max.y = b.y;
	} else {
		min.y = b.y;
		max.y = a.y;
	}

	return Aabb{ min, max };
}

auto Aabb::fromPoints(View<const Vec2> points) -> Aabb {
	Vec2 min{ std::numeric_limits<float>::infinity() }, max{ -std::numeric_limits<float>::infinity() };

	for (const auto& point : points) {
		min = min.min(point);
		max = max.max(point);
	}

	return Aabb{ min, max };
}

auto Aabb::size() const -> Vec2 {
	return max - min;
}

auto Aabb::contains(Vec2 p) const -> bool {
	return p.x >= min.x && p.x <= max.x
		&& p.y >= min.y && p.y <= max.y;
}

auto Aabb::contains(const Aabb& aabb) const -> bool {
	const auto shrinkedMax = max - (aabb.max - aabb.min);

	if (shrinkedMax.x - min.x < 0.0f || shrinkedMax.y - min.y < 0.0f)
		// aabb is bigger than this
		return false;

	return Aabb{ min, shrinkedMax }.contains(aabb.min);
}

auto Aabb::combined(const Aabb& aabb) const -> Aabb {
	return Aabb{ min.min(aabb.min), max.max(aabb.max) };
}

auto Aabb::extended(Vec2 point) const -> Aabb {
	return Aabb{ min.min(point), max.max(point) };
}

auto Aabb::addedPadding(float padding) const -> Aabb {
	return Aabb{ min - Vec2{ padding }, max + Vec2{ padding } };
}

Aabb Aabb::translated(Vec2 translation) const {
	return Aabb(min + translation, max + translation);
}

Aabb Aabb::intersection(const Aabb& aabb) const {
	const auto mi = Vec2(std::max(aabb.min.x, min.x), std::max(aabb.min.y, min.y));
	const auto ma = Vec2(std::min(aabb.max.x, max.x), std::min(aabb.max.y, max.y));

	if (mi.x > ma.x || mi.y > ma.y) {
		return Aabb(Vec2(0.0f), Vec2(0.0f));
	}
	return Aabb(mi, ma);
}

auto Aabb::area() const -> float {
	const auto s = size();
	return s.x * s.y;
}

auto Aabb::collides(const Aabb& other) const -> bool {
	return min.x <= other.max.x && max.x >= other.min.x
		&& min.y <= other.max.y && max.y >= other.min.y;
}

auto Aabb::rayHits(Vec2 start, Vec2 end) const -> bool {
	const auto dir = end - start;
	float t1 = (min.x - start.x) / dir.x;
	float t2 = (max.x - start.x) / dir.x;
	float t3 = (min.y - start.y) / dir.y;
	float t4 = (max.y - start.y) / dir.y;

	float tMin = std::max(std::min(t1, t2), std::min(t3, t4));
	float tMax = std::min(std::max(t1, t2), std::max(t3, t4));

	// Intersection behind.
	if (tMax < 0)
		return false;

	// No hit.
	if (tMin > tMax)
		return false;

	return true;
}

auto Aabb::center() const -> Vec2 {
	return min + (max - min) / 2.0f;
}

auto Aabb::getCorners() const -> std::array<Vec2, 4> {
	return {
		min,
		Vec2{ max.x, min.y },
		max,
		Vec2{ min.x, max.y },
	};
}
