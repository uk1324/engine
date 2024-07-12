#pragma once

#include <View.hpp>
#include "Aabb.hpp"

Aabb circleAabb(Vec2 center, f32 radius);
Aabb transformedPolygonAabb(View<const Vec2> vertices, Vec2 translation, f32 rotation);