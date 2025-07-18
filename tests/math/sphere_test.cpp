/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#include <gtest/gtest.h>
#include <test_helpers.hpp>

#include <gleam/math/matrix4.hpp>
#include <gleam/math/sphere.hpp>
#include <gleam/math/vector3.hpp>

#include <cassert>

#pragma region Constructors

TEST(Sphere, DefaultConstructor) {
    constexpr auto sphere = gleam::Sphere {};

    EXPECT_VEC3_EQ(sphere.center, gleam::Vector3::Zero());
    EXPECT_FLOAT_EQ(sphere.radius, -1.0f);

    static_assert(sphere.center == gleam::Vector3::Zero());
    static_assert(sphere.radius == -1.0f);
}

TEST(Sphere, ConstructorParameterized) {
    constexpr auto sphere = gleam::Sphere {
        {1.0f, 1.0f, 1.0f},
        2.0f
    };

    EXPECT_VEC3_EQ(sphere.center, {1.0f, 1.0f, 1.0f});
    EXPECT_FLOAT_EQ(sphere.radius, 2.0f);

    static_assert(sphere.center == gleam::Vector3 {1.0f, 1.0f, 1.0f});
    static_assert(sphere.radius == 2.0f);
}

#pragma endregion

#pragma region Empty State

TEST(Sphere, Reset) {
    auto s1 = gleam::Sphere {1.0f, 2.0f};

    s1.Reset();

    EXPECT_TRUE(s1.IsEmpty());
    EXPECT_VEC3_EQ(s1.center, gleam::Vector3::Zero());
    EXPECT_FLOAT_EQ(s1.radius, -1.0f);

    constexpr auto s2 = []() {
        auto s = gleam::Sphere {1.0f, 2.0f};
        s.Reset();
        return s;
    }();

    static_assert(s2.IsEmpty());
    static_assert(s2.center == gleam::Vector3::Zero());
    static_assert(s2.radius == -1.0f);
}

TEST(Sphere, IsEmptyTrue) {
    constexpr auto sphere = gleam::Sphere {};

    EXPECT_TRUE(sphere.IsEmpty());

    static_assert(sphere.IsEmpty());
}

TEST(Sphere, IsEmptyFalse) {
    constexpr auto sphere = gleam::Sphere {1.0f, 1.0f};

    EXPECT_FALSE(sphere.IsEmpty());

    static_assert(!sphere.IsEmpty());
}

#pragma endregion

#pragma region Expand with Point

TEST(Sphere, ExpandWithPointEmptySphere) {
    auto s1 = gleam::Sphere {};

    s1.ExpandWithPoint({1.0f, 1.0f, 1.0f});

    EXPECT_VEC3_EQ(s1.center, {1.0f, 1.0f, 1.0f});
    EXPECT_FLOAT_EQ(s1.radius, 0.0f);

    constexpr auto s2 = []() {
        auto s = gleam::Sphere {};
        s.ExpandWithPoint({1.0f, 1.0f, 1.0f});
        return s;
    }();

    static_assert(s2.center == gleam::Vector3 {1.0f, 1.0f, 1.0f});
    static_assert(s2.radius == 0.0f);
}

TEST(Sphere, ExpandWithPointInsideSphere) {
    auto s1 = gleam::Sphere {gleam::Vector3::Zero(), 5.0f};

    s1.ExpandWithPoint({1.0f, 1.0f, 1.0f});

    EXPECT_VEC3_EQ(s1.center, gleam::Vector3::Zero());
    EXPECT_FLOAT_EQ(s1.radius, 5.0f);

    constexpr auto s2 = []() {
        auto s = gleam::Sphere {gleam::Vector3::Zero(), 5.0f};
        s.ExpandWithPoint({1.0f, 1.0f, 1.0f});
        return s;
    }();

    static_assert(s2.center == gleam::Vector3::Zero());
    static_assert(s2.radius == 5.0f);
}

TEST(Sphere, ExpandWithPointOnSphereSurface) {
    auto s1 = gleam::Sphere {gleam::Vector3::Zero(), 1.0f};

    s1.ExpandWithPoint({1.0f, 0.0f, 0.0f});

    EXPECT_VEC3_EQ(s1.center, gleam::Vector3::Zero());
    EXPECT_FLOAT_EQ(s1.radius, 1.0f);

    constexpr auto s2 = []() {
        auto s = gleam::Sphere {gleam::Vector3::Zero(), 1.0f};
        s.ExpandWithPoint({1.0f, 0.0f, 0.0f});
        return s;
    }();

    static_assert(s2.center == gleam::Vector3::Zero());
    static_assert(s2.radius == 1.0f);
}

TEST(Sphere, ExpandWithPointOutsideSphere) {
    auto s1 = gleam::Sphere {gleam::Vector3::Zero(), 1.0f};

    s1.ExpandWithPoint({2.0f, 0.0f, 0.0f});

    EXPECT_VEC3_NEAR(s1.center, {0.5f, 0.0f, 0.0f}, 1e-4);
    EXPECT_NEAR(s1.radius, 1.5f, 1e-4);

    constexpr auto s2 = []() {
        auto s = gleam::Sphere {gleam::Vector3::Zero(), 1.0f};
        s.ExpandWithPoint({2.0f, 0.0f, 0.0f});
        return s;
    }();

    static_assert(ApproxEqual(s2.center.x, 0.5f));
    static_assert(ApproxEqual(s2.center.y, 0.0f));
    static_assert(ApproxEqual(s2.center.z, 0.0f));
    static_assert(ApproxEqual(s2.radius, 1.5f));
}

#pragma endregion

#pragma region Apply Transform

TEST(Sphere, TransformWithIdentityMatrix) {
    auto s1 = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
    constexpr auto transform = gleam::Matrix4::Identity();

    s1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(s1.center, {1.0f, 2.0f, 3.0f});
    EXPECT_NEAR(s1.radius, 4.0f, 1e-4);

    constexpr auto s2 = [&transform]() {
        auto s = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
        s.ApplyTransform(transform);
        return s;
    }();

    static_assert(s2.center == gleam::Vector3 {1.0f, 2.0f, 3.0f});
    static_assert(ApproxEqual(s2.radius, 4.0f));
}

TEST(Sphere, TransformWithTranslation) {
    auto s1 = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
    constexpr auto transform = gleam::Matrix4 {
        1.0f, 0.0f, 0.0f, 2.0f,
        0.0f, 1.0f, 0.0f, 3.0f,
        0.0f, 0.0f, 1.0f, 4.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    s1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(s1.center, {3.0f, 5.0f, 7.0f});
    EXPECT_NEAR(s1.radius, 4.0f, 1e-4);

    constexpr auto s2 = [&transform]() {
        auto s = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
        s.ApplyTransform(transform);
        return s;
    }();

    static_assert(s2.center == gleam::Vector3 {3.0f, 5.0f, 7.0f});
    static_assert(ApproxEqual(s2.radius, 4.0f));
}

TEST(Sphere, TransformWithScale) {
    auto s1 = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
    constexpr auto transform = gleam::Matrix4 {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    s1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(s1.center, {2.0f, 4.0f, 6.0f});
    EXPECT_NEAR(s1.radius, 8.0f, 1e-4);

    constexpr auto s2 = [&transform]() {
        auto s = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
        s.ApplyTransform(transform);
        return s;
    }();

    static_assert(s2.center == gleam::Vector3 {2.0f, 4.0f, 6.0f});
    static_assert(ApproxEqual(s2.radius, 8.0f));
}

TEST(Sphere, TransformWithRotation) {
    auto s1 = gleam::Sphere {{1.0f, 0.0f, 0.0f}, 4.0f};

    // Rotate 90 degrees around the z-axis
    constexpr auto transform = gleam::Matrix4 {
        0.0f, -1.0f, 0.0f, 0.0f,
        1.0f,  0.0f, 0.0f, 0.0f,
        0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 0.0f, 1.0f
    };

    s1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(s1.center, {0.0f, 1.0f, 0.0f});
    EXPECT_NEAR(s1.radius, 4.0f, 1e-4);

    constexpr auto s2 = [&transform]() {
        auto s = gleam::Sphere {{1.0f, 0.0f, 0.0f}, 4.0f};
        s.ApplyTransform(transform);
        return s;
    }();

    static_assert(s2.center == gleam::Vector3 {0.0f, 1.0f, 0.0f});
    static_assert(ApproxEqual(s2.radius, 4.0f));
}

#pragma endregion

#pragma region Translate

TEST(Sphere, Translate) {
    auto s1 = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
    s1.Translate({1.0f, 2.0f, 3.0f});

    EXPECT_VEC3_EQ(s1.center, {2.0f, 4.0f, 6.0f});
    EXPECT_FLOAT_EQ(s1.radius, 4.0f);

    constexpr auto s2 = []() {
        auto s = gleam::Sphere {{1.0f, 2.0f, 3.0f}, 4.0f};
        s.Translate({1.0f, 2.0f, 3.0f});
        return s;
    }();

    static_assert(s2.center == gleam::Vector3 {2.0f, 4.0f, 6.0f});
    static_assert(s2.radius == 4.0f);
}

#pragma endregion