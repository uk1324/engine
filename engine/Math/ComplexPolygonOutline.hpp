#pragma once

#include <vector>
#include <RefOptional.hpp>
#include "Vec2.hpp"
#include <View.hpp>

namespace { static thread_local std::vector<Vec2> complexPolygonOtlineResult; }
std::optional<const std::vector<Vec2>&> complexPolygonOutline(View<const Vec2> vertices, std::vector<Vec2>& result = complexPolygonOtlineResult);