/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#include <gtest/gtest.h>
#include <test_helpers.hpp>

#include <cassert>
#include <limits>

#include <gleam/math/box3.hpp>
#include <gleam/math/matrix4.hpp>
#include <gleam/math/vector3.hpp>

#pragma region Constructors

TEST(Box3, DefaultConstructor) {
    constexpr auto box = gleam::Box3 {};

    EXPECT_VEC3_EQ(box.min, std::numeric_limits<float>::max());
    EXPECT_VEC3_EQ(box.max, std::numeric_limits<float>::lowest());

    static_assert(box.min == std::numeric_limits<float>::max());
    static_assert(box.max == std::numeric_limits<float>::lowest());
}

TEST(Vector3, ConstructorParameterized) {
    constexpr auto box = gleam::Box3 {
        {-1.0f, -1.0f, -1.0f},
        {1.0f, 1.0f, 1.0f}
    };

    EXPECT_VEC3_EQ(box.min, {-1.0f, -1.0f, -1.0f});
    EXPECT_VEC3_EQ(box.max, {1.0f, 1.0f, 1.0f});

    static_assert(box.min == gleam::Vector3 {-1.0f, -1.0f, -1.0f});
    static_assert(box.max == gleam::Vector3 {1.0f, 1.0f, 1.0f});
}

#pragma endregion

#pragma region Empty State

TEST(Box3, Reset) {
    auto b1 = gleam::Box3 {
        {-1.0f, -1.0f, -1.0f},
        {1.0f, 1.0f, 1.0f}
    };
    b1.Reset();

    EXPECT_TRUE(b1.IsEmpty());
    EXPECT_VEC3_EQ(b1.min, std::numeric_limits<float>::max());
    EXPECT_VEC3_EQ(b1.max, std::numeric_limits<float>::lowest());

    constexpr auto b2 = []() {
        auto b = gleam::Box3 {
            {-1.0f, -1.0f, -1.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.Reset();
        return b;
    }();

    static_assert(b2.IsEmpty());
    static_assert(b2.min == std::numeric_limits<float>::max());
    static_assert(b2.max == std::numeric_limits<float>::lowest());
}

TEST(Box3, IsEmptyTrue) {
    constexpr auto box = gleam::Box3 {
        {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f}
    };

    EXPECT_TRUE(box.IsEmpty());

    static_assert(box.IsEmpty());
}

TEST(Box3, IsEmptyFalse) {
    constexpr auto box = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };

    EXPECT_FALSE(box.IsEmpty());

    static_assert(!box.IsEmpty());
}

#pragma endregion

#pragma region Center

TEST(Box3, CenterWithNegativeAndPositiveValues) {
    constexpr auto box = gleam::Box3 {
        {0.0f, 0.0f, -1.0f},
        {1.0f, 1.0f, 2.0f}
    };

    EXPECT_VEC3_EQ(box.Center(), {0.5f, 0.5f, 0.5f});

    static_assert(box.Center() == gleam::Vector3 {0.5f, 0.5f, 0.5f});
}

TEST(Box3, CenterWithSameValues) {
    constexpr auto box = gleam::Box3 {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };

    EXPECT_VEC3_EQ(box.Center(), {1.0f, 1.0f, 1.0f});

    static_assert(box.Center() == gleam::Vector3 {1.0f, 1.0f, 1.0f});
}

TEST(Box3, CenterWithZeroValues) {
    constexpr auto box = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };

    EXPECT_VEC3_EQ(box.Center(), {0.0f, 0.0f, 0.0f});

    static_assert(box.Center() == gleam::Vector3 {0.0f, 0.0f, 0.0f});
}

#pragma endregion

#pragma region Expand with Point

TEST(Box3, ExpandWithPoint) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };

    b1.ExpandWithPoint({1.0f, 2.0f, -2.0f});

    EXPECT_VEC3_EQ(b1.min, {0.0f, 0.0f, -2.0f});
    EXPECT_VEC3_EQ(b1.max, {1.0f, 2.0f, 1.0f});

    constexpr auto b2 = []() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.ExpandWithPoint({1.0f, 2.0f, -2.0f});
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {0.0f, 0.0f, -2.0f});
    static_assert(b2.max == gleam::Vector3 {1.0f, 2.0f, 1.0f});
}

TEST(Box3, ExpandWithPointInsideBox) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };

    b1.ExpandWithPoint({0.5f, 0.5f, 0.5f});

    EXPECT_VEC3_EQ(b1.min, {0.0f, 0.0f, 0.0f});
    EXPECT_VEC3_EQ(b1.max, {1.0f, 1.0f, 1.0f});

    constexpr auto b2 = []() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.ExpandWithPoint({0.5f, 0.5f, 0.5f});
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {0.0f, 0.0f, 0.0f});
    static_assert(b2.max == gleam::Vector3 {1.0f, 1.0f, 1.0f});
}

TEST(Box3, ExpandWithPointOnMinMax) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };

    b1.ExpandWithPoint({0.0f, 0.0f, 0.0f});
    b1.ExpandWithPoint({1.0f, 1.0f, 1.0f});

    EXPECT_VEC3_EQ(b1.min, {0.0f, 0.0f, 0.0f});
    EXPECT_VEC3_EQ(b1.max, {1.0f, 1.0f, 1.0f});

    constexpr auto b2 = []() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.ExpandWithPoint({0.0f, 0.0f, 0.0f});
        b.ExpandWithPoint({1.0f, 1.0f, 1.0f});
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {0.0f, 0.0f, 0.0f});
    static_assert(b2.max == gleam::Vector3 {1.0f, 1.0f, 1.0f});
}

TEST(Box3, ExpandWithMultiplePoints) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };

    b1.ExpandWithPoint({2.0f, 2.0f, 2.0f});
    b1.ExpandWithPoint({-1.0f, -1.0f, -1.0f});

    EXPECT_VEC3_EQ(b1.min, {-1.0f, -1.0f, -1.0f});
    EXPECT_VEC3_EQ(b1.max, {2.0f, 2.0f, 2.0f});

    constexpr auto b2 = []() {
        auto b = gleam::Box3 {
            {2.0f, 2.0f, 2.0f},
            {-1.0f, -1.0f, -1.0f}
        };
        b.ExpandWithPoint({2.0f, 2.0f, 2.0f});
        b.ExpandWithPoint({-1.0f, -1.0f, -1.0f});
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {-1.0f, -1.0f, -1.0f});
    static_assert(b2.max == gleam::Vector3 {2.0f, 2.0f, 2.0f});
}

#pragma endregion

#pragma region Apply Transform

TEST(Box3, TransformWithIdentityMatrix) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };
    constexpr auto transform = gleam::Matrix4::Identity();

    b1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(b1.min, {0.0f, 0.0f, 0.0f});
    EXPECT_VEC3_EQ(b1.max, {1.0f, 1.0f, 1.0f});

    constexpr auto b2 = [&transform]() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.ApplyTransform(transform);
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {0.0f, 0.0f, 0.0f});
    static_assert(b2.max == gleam::Vector3 {1.0f, 1.0f, 1.0f});
}

TEST(Box3, TransformWithTranslation) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };
    constexpr auto transform = gleam::Matrix4 {
        1.0f, 0.0f, 0.0f, 2.0f,
        0.0f, 1.0f, 0.0f, 3.0f,
        0.0f, 0.0f, 1.0f, 4.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    b1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(b1.min, {2.0f, 3.0f, 4.0f});
    EXPECT_VEC3_EQ(b1.max, {3.0f, 4.0f, 5.0f});

    constexpr auto b2 = [&transform]() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.ApplyTransform(transform);
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {2.0f, 3.0f, 4.0f});
    static_assert(b2.max == gleam::Vector3 {3.0f, 4.0f, 5.0f});
}

TEST(Box3, TransformWithScale) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };
    constexpr auto transform = gleam::Matrix4 {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 3.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    b1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(b1.min, {0.0f, 0.0f, 0.0f});
    EXPECT_VEC3_EQ(b1.max, {2.0f, 1.0f, 3.0f});

    constexpr auto b2 = [&transform]() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.ApplyTransform(transform);
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {0.0f, 0.0f, 0.0f});
    static_assert(b2.max == gleam::Vector3 {2.0f, 1.0f, 3.0f});
}

TEST(Box3, TransformWithRotation) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };
    // Rotate 90 degrees around the z-axis
    constexpr auto transform = gleam::Matrix4 {
        0.0f, -1.0f, 0.0f, 0.0f,
        1.0f,  0.0f, 0.0f, 0.0f,
        0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 0.0f, 1.0f
    };

    b1.ApplyTransform(transform);

    EXPECT_VEC3_EQ(b1.min, {-1.0f, 0.0f, 0.0f});
    EXPECT_VEC3_EQ(b1.max, {0.0f, 1.0f, 1.0f});

    constexpr auto b2 = [&transform]() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.ApplyTransform(transform);
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {-1.0f, 0.0f, 0.0f});
    static_assert(b2.max == gleam::Vector3 {0.0f, 1.0f, 1.0f});
}

#pragma endregion

#pragma region Translate

TEST(Box3, Translate) {
    auto b1 = gleam::Box3 {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };

    b1.Translate({1.0f, 2.0f, 3.0f});

    EXPECT_VEC3_EQ(b1.min, {1.0f, 2.0f, 3.0f});
    EXPECT_VEC3_EQ(b1.max, {2.0f, 3.0f, 4.0f});

    constexpr auto b2 = []() {
        auto b = gleam::Box3 {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };
        b.Translate({1.0f, 2.0f, 3.0f});
        return b;
    }();

    static_assert(b2.min == gleam::Vector3 {1.0f, 2.0f, 3.0f});
    static_assert(b2.max == gleam::Vector3 {2.0f, 3.0f, 4.0f});
}

#pragma endregion