#include "Sphere.hpp"

std::optional<f32> raySphereIntersection(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, f32 sphereRadius) {
    const auto oc = rayOrigin - sphereCenter;
    const auto a = dot(rayDirection, rayDirection);
    const auto half_b = dot(oc, rayDirection);
    const auto c = dot(oc, oc) - sphereRadius * sphereRadius;
    const auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0) {
        return std::nullopt;
    }

    const auto sqrt_discriminant = sqrt(discriminant);

    const auto root = (-half_b - sqrt_discriminant) / a;

    if (root < 0.0f) {
        return std::nullopt;
    }
    return root;
}

std::optional<f32> raySphereIntersection(const Ray3& ray, Vec3 sphereCenter, f32 sphereRadius) {
    return raySphereIntersection(ray.origin, ray.direction, sphereCenter, sphereRadius);
}
