#include "Ray3.hpp"

Ray3::Ray3(Vec3 origin, Vec3 direction)
    : origin(origin)
    , direction(direction) {}

Vec3 Ray3::at(f32 t) const {
    return rayAt(t, origin, direction);
}

Vec3 rayAt(f32 t, Vec3 rayOrigin, Vec3 rayDirection) {
    return rayOrigin + t * rayDirection;
}