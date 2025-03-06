// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#include "engine/math/plane.hpp"

namespace engine {

auto Plane::DistanceToPoint(const Vector3& point) const -> float {
    return Dot(Normalize(normal_), point) - distance_;
}

auto Plane::DistanceToSphere(const Sphere& sphere) const -> float {
    return DistanceToPoint(sphere.Center()) - sphere.Radius();
}

}