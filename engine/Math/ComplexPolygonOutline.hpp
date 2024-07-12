#pragma once

#include <vector>
#include <optional>
#include "Vec2.hpp"
#include <View.hpp>

namespace { static thread_local std::vector<Vec2> complexPolygonOtlineResult; }
auto complexPolygonOutline(View<const Vec2> vertices, std::vector<Vec2>& result = complexPolygonOtlineResult) -> const std::vector<Vec2>&;