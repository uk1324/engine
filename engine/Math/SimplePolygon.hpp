#include "Vec2.hpp"

#include <vector>
#include <View.hpp>

auto simplePolygonSignedArea(View<const Vec2> verts) -> float;
auto simplePolygonArea(View<const Vec2> verts) -> float;
auto simplePolygonIsClockwise(View<const Vec2> verts) -> bool;