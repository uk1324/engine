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

    // This detects only the intersection outside the sphere. If the ray starts inside the sphere it won't detect an intersection.
    /*if (root < 0.0f) {
        return std::nullopt;
    }*/
    if (root < 0.0f) {
        const auto r = (-half_b + sqrt_discriminant) / a;
        if (r < 0.0f) {
            return std::nullopt;
        } 
        return r;
    }
    return root;
}

std::optional<f32> raySphereIntersection(Vec3 rayOrigin, Vec3 rayDirection, const Sphere& sphere) {
    return raySphereIntersection(rayOrigin, rayDirection, sphere.center, sphere.radius);
}

std::optional<Vec3> raySphereIntersectionPoint(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, f32 sphereRadius) {
    const auto i = raySphereIntersection(rayOrigin, rayDirection, sphereCenter, sphereRadius);
    if (i.has_value()) {
        return rayAt(*i, rayOrigin, rayDirection);
    }
    return std::nullopt;
}

std::optional<f32> raySphereIntersection(const Ray3& ray, Vec3 sphereCenter, f32 sphereRadius) {
    return raySphereIntersection(ray.origin, ray.direction, sphereCenter, sphereRadius);
}

Sphere::Sphere(Vec3 center, f32 radius)
    : center(center)
    , radius(radius) {}

Sphere Sphere::thoughPoints(Vec3 af, Vec3 bf, Vec3 cf, Vec3 df) {
    const auto a = Vec3T<f64>(af);
    const auto b = Vec3T<f64>(bf);
    const auto c = Vec3T<f64>(cf);
    const auto d = Vec3T<f64>(df);
    #define U(a,b,c,d,e,f,g,h) (a.z - b.z)*(c.x*d.y - d.x*c.y) - (e.z - f.z)*(g.x*h.y - h.x*g.y)
    #define D(x,y,a,b,c) (a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y))
    #define E(x,y) ((ra*D(x,y,b,c,d) - rb*D(x,y,c,d,a) + rc*D(x,y,d,a,b) - rd*D(x,y,a,b,c)) / uvw)
    double u = U(a,b,c,d,b,c,d,a);
    double v = U(c,d,a,b,d,a,b,c);
    double w = U(a,c,d,b,b,d,a,c);
    double uvw = 2 * (u + v + w);
    if (uvw == 0.0) {
        // Oops.  The points are coplanar.
    }
    auto sq = [] (Vec3 p) { return p.x*p.x + p.y*p.y + p.z*p.z; };
    double ra = sq(a);
    double rb = sq(b);
    double rc = sq(c);
    double rd = sq(d);
    double x0 = E(y,z);
    double y0 = E(z,x);
    double z0 = E(x,y);
    double radius = sqrt(sq(Vec3T<f64>(a.x - x0, a.y - y0, a.z - z0)));
    return Sphere(Vec3(f32(x0), f32(y0), f32(z0)), f32(radius));
    #undef U
    #undef D
    #undef E
}
