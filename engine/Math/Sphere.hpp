#pragma once

#include "Ray3.hpp"
#include <optional>

std::optional<f32> raySphereIntersection(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, f32 sphereRadius);
std::optional<f32> raySphereIntersection(const Ray3& ray, Vec3 sphereCenter, f32 sphereRadius);