#pragma once

#include <View.hpp>
#include <engine/Math/Vec2.hpp>

// Can return zero area shapes.
// Can create self intersections.
std::vector<Vec2> polygonDouglassPeckerSimplify(View<const Vec2> verts, float maxDistanceFromLine);