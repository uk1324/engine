#include "Tri3.hpp"

Vec3 triCenter(Vec3 v0, Vec3 v1, Vec3 v2) {
    return (v0 + v1 + v2) / 3.0f;
}

Vec3 triCenter(const Vec3* v) {
    return triCenter(v[0], v[2], v[1]);
}

std::optional<RayTriIntersection> rayTriIntersection(Vec3 rayOrigin, Vec3 rayDirection, Vec3 v0, Vec3 v1, Vec3 v2) {
    constexpr auto epsilon = std::numeric_limits<f32>::epsilon();
    Vec3 v0v1 = v1 - v0;
    Vec3 v0v2 = v2 - v0;
    Vec3 pvec = cross(rayDirection, v0v2);
    float det = dot(v0v1, pvec);
#ifdef CULLING
    // If the determinant is negative, the triangle is back-facing.
    // If the determinant is close to 0, the ray misses the triangle.
    if (det < kEpsilon) return false;
#else
    // If det is close to 0, the ray and triangle are parallel.
    if (fabs(det) < epsilon) return std::nullopt;
#endif
    float invDet = 1 / det;

    Vec3 tvec = rayOrigin - v0;
    const auto u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return std::nullopt;

    Vec3 qvec = cross(tvec, v0v1);
    const auto v = dot(rayDirection, qvec) * invDet;
    if (v < 0 || u + v > 1) return std::nullopt;

    const auto t = dot(v0v2, qvec) * invDet;

    return RayTriIntersection{
        .position = rayOrigin + t * rayDirection,
        .barycentricCoordinates = Vec3(1.0f - u - v, u, v),
        .t = t
    };
}

std::optional<RayTriIntersection> rayTriIntersection(Vec3 rayOrigin, Vec3 rayDirection, const Vec3* v) {
    return rayTriIntersection(rayOrigin, rayDirection, v[0], v[1], v[2]);
}

f32 triArea(Vec3 v0, Vec3 v1, Vec3 v2) {
    return cross(v1 - v0, v2 - v0).length();
}

f32 triArea(const Vec3* v) {
    return triArea(v[0], v[1], v[2]);
}

Vec3 uniformRandomPointOnTri(Vec3 v0, Vec3 v1, Vec3 v2, f32 r0, f32 r1) {
    // https://stackoverflow.com/questions/19654251/random-point-inside-triangle-inside-java
    return
        (1.0f - sqrt(r0)) * v0 +
        (sqrt(r0) * (1.0f - r1)) * v1 +
        (r1 * sqrt(r0)) * v2;
}

Vec3 uniformRandomPointOnTri(const Vec3* v, f32 r0, f32 r1) {
    return uniformRandomPointOnTri(v[0], v[1], v[2], r0, r1);
}

Vec2 uniformRandomPointOnTri(Vec2 v0, Vec2 v1, Vec2 v2, f32 r0, f32 r1) {
    return
        (1.0f - sqrt(r0)) * v0 +
        (sqrt(r0) * (1.0f - r1)) * v1 +
        (r1 * sqrt(r0)) * v2;
}

Vec2 uniformRandomPointOnTri(const Vec2* v, f32 r0, f32 r1) {
    return uniformRandomPointOnTri(v[0], v[1], v[2], r0, r1);
}

Vec3 triNormal(const Vec3* v) {
    return cross(v[1] - v[0], v[2] - v[0]).normalized();
}
