/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#include <gtest/gtest.h>
#include <test_helpers.hpp>

#include <gleam/math/transform2.hpp>
#include <gleam/math/utilities.hpp>

#include <cassert>

#pragma region Mutators

TEST(Transform2, SetPosition) {
    auto t1 = gleam::Transform2 {};
    t1.SetPosition({2.0f, 1.0f});

    EXPECT_VEC2_EQ(t1.GetPosition(), {2.0f, 1.0f});
    EXPECT_MAT3_EQ(t1.Get(), {
        1.0f, 0.0f, 2.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto t2 = []() {
        auto t = gleam::Transform2 {};
        t.SetPosition({2.0f, 1.0f});
        return t;
    }();

    static_assert(t2.GetPosition().x == 2.0f);
    static_assert(t2.GetPosition().y == 1.0f);
}

TEST(Transform2, SetScale) {
    auto t1 = gleam::Transform2 {};
    t1.SetScale({2.0f, 1.0f});

    EXPECT_VEC2_EQ(t1.GetScale(), {2.0f, 1.0f});
    EXPECT_MAT3_EQ(t1.Get(), {
        2.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto t2 = []() {
        auto t = gleam::Transform2 {};
        t.SetScale({2.0f, 1.0f});
        return t;
    }();

    static_assert(t2.GetScale().x == 2.0f);
    static_assert(t2.GetScale().y == 1.0f);
}

TEST(Transform2, SetRotation) {
    auto t = gleam::Transform2 {};
    t.SetRotation(gleam::math::pi_over_2);

    constexpr auto c = gleam::math::Cos(gleam::math::pi_over_2);
    constexpr auto s = gleam::math::Sin(gleam::math::pi_over_2);

    EXPECT_MAT3_EQ(t.Get(), {
        c, -s, 0.0f,
        s, c, 0.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto m = []() {
        auto t = gleam::Transform2 {};
        t.SetRotation(gleam::math::pi_over_2);
        return t.Get();
    }();

    static_assert(m[0].x == c);
    static_assert(m[0].y == s);
    static_assert(m[1].x == -s);
    static_assert(m[1].y == c);
}

TEST(Transform2, MultipleTransformations) {
    auto t = gleam::Transform2 {};
    t.SetCenter({0.5f, 0.5f});
    t.SetPosition({2.0f, 3.0f});
    t.SetScale({2.0f, 2.0f});
    t.SetRotation(gleam::math::pi_over_2);

    constexpr auto c = gleam::math::Cos(gleam::math::pi_over_2) * 2.0f;
    constexpr auto s = gleam::math::Sin(gleam::math::pi_over_2) * 2.0f;

    EXPECT_VEC2_EQ(t.GetCenter(), {0.5f, 0.5f});
    EXPECT_VEC2_EQ(t.GetPosition(), {2.0f, 3.0f});
    EXPECT_VEC2_EQ(t.GetScale(), {2.0f, 2.0f});
    EXPECT_EQ(t.GetRotation(), gleam::math::pi_over_2);
    EXPECT_MAT3_EQ(t.Get(), {
        c, -s, 3.5f,
        s, c, 2.5f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto m = []() {
        auto t = gleam::Transform2 {};
        t.SetCenter({0.5f, 0.5f});
        t.SetPosition({2.0f, 3.0f});
        t.SetScale({2.0f, 2.0f});
        t.SetRotation(gleam::math::pi_over_2);
        return t.Get();
    }();

    static_assert(m[0].x == c);
    static_assert(m[0].y == s);
    static_assert(m[1].x == -s);
    static_assert(m[1].y == c);
    static_assert(m[2].x == 3.5f);
    static_assert(m[2].y == 2.5f);
}

#pragma endregion

#pragma region Cumulative Transformations

TEST(Transform2, Translate) {
    auto t1 = gleam::Transform2 {};
    t1.Translate({2.0f, 1.0f});

    EXPECT_VEC2_EQ(t1.GetPosition(), {2.0f, 1.0f});
    EXPECT_MAT3_EQ(t1.Get(), {
        1.0f, 0.0f, 2.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto t2 = []() {
        auto t = gleam::Transform2 {};
        t.Translate({2.0f, 1.0f});
        return t;
    }();

    static_assert(t2.GetPosition().x == 2.0f);
    static_assert(t2.GetPosition().y == 1.0f);
}

TEST(Transform2, Scale) {
    auto t = gleam::Transform2 {};
    t.Scale({2.0f, 1.0f});

    EXPECT_VEC2_EQ(t.GetScale(), {2.0f, 1.0f});
    EXPECT_MAT3_EQ(t.Get(), {
        2.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto t2 = []() {
        auto t = gleam::Transform2 {};
        t.Scale({2.0f, 1.0f});
        return t;
    }();

    static_assert(t2.GetScale().x == 2.0f);
    static_assert(t2.GetScale().y == 1.0f);
}

TEST(Transform2, Rotate) {
    auto t = gleam::Transform2 {};
    t.Rotate(gleam::math::pi_over_2);

    constexpr auto c = gleam::math::Cos(gleam::math::pi_over_2);
    constexpr auto s = gleam::math::Sin(gleam::math::pi_over_2);

    EXPECT_MAT3_EQ(t.Get(), {
        c, -s, 0.0f,
        s, c, 0.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto m = []() {
        auto t = gleam::Transform2 {};
        t.Rotate(gleam::math::pi_over_2);
        return t.Get();
    }();

    static_assert(m[0].x == c);
    static_assert(m[0].y == s);
    static_assert(m[1].x == -s);
    static_assert(m[1].y == c);
}

TEST(Transform2, TransformationsWithOffset) {
    auto t = gleam::Transform2 {};
    t.SetCenter({0.5, 0.5f});
    t.Translate({2.0f, 3.0f});
    t.Scale({2.0f, 2.0f});
    t.Rotate(gleam::math::pi_over_2);

    constexpr auto c = gleam::math::Cos(gleam::math::pi_over_2) * 2.0f;
    constexpr auto s = gleam::math::Sin(gleam::math::pi_over_2) * 2.0f;

    EXPECT_VEC2_EQ(t.GetCenter(), {0.5f, 0.5f});
    EXPECT_VEC2_EQ(t.GetPosition(), {2.0f, 3.0f});
    EXPECT_VEC2_EQ(t.GetScale(), {2.0f, 2.0f});
    EXPECT_EQ(t.GetRotation(), gleam::math::pi_over_2);
    EXPECT_MAT3_EQ(t.Get(), {
        c, -s, 3.5f,
        s, c, 2.5f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto m = []() {
        auto t = gleam::Transform2 {};
        t.SetCenter({0.5f, 0.5f});
        t.SetPosition({2.0f, 3.0f});
        t.SetScale({2.0f, 2.0f});
        t.SetRotation(gleam::math::pi_over_2);
        return t.Get();
    }();

    static_assert(m[0].x == c);
    static_assert(m[0].y == s);
    static_assert(m[1].x == -s);
    static_assert(m[1].y == c);
    static_assert(m[2].x == 3.5f);
    static_assert(m[2].y == 2.5f);
}

#pragma endregion

#pragma region Local-Space Translation

TEST(Transform2, TranslateBeforeRotation) {
    auto t = gleam::Transform2 {};
    t.Translate({0.0f, 1.0f});
    t.Rotate(gleam::math::pi_over_2);

    constexpr auto c = gleam::math::Cos(gleam::math::pi_over_2);
    constexpr auto s = gleam::math::Sin(gleam::math::pi_over_2);

    EXPECT_VEC2_EQ(t.GetPosition(), {0.0f, 1.0f});
    EXPECT_MAT3_EQ(t.Get(), {
        c, -s, 0.0f,
        s,  c, 1.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto m = []() {
        auto t = gleam::Transform2 {};
        t.Translate({0.0f, 1.0f});
        t.Rotate(gleam::math::pi_over_2);
        return t.Get();
    }();

    static_assert(m[0].x == c);
    static_assert(m[0].y == s);
    static_assert(m[1].x == -s);
    static_assert(m[1].y == c);
    static_assert(m[2].x == 0.0f);
    static_assert(m[2].y == 1.0f);
}

TEST(Transform2, TranslateAfterRotation) {
    auto t = gleam::Transform2 {};
    t.Rotate(gleam::math::pi_over_2);
    t.Translate({0.0f, 1.0f});

    constexpr auto c = gleam::math::Cos(gleam::math::pi_over_2);
    constexpr auto s = gleam::math::Sin(gleam::math::pi_over_2);

    EXPECT_VEC2_NEAR(t.GetPosition(), {-1.0f, 0.0f}, 0.001f);
    EXPECT_MAT3_EQ(t.Get(), {
        c, -s, -1.0f,
        s,  c,  0.0f,
        0.0f, 0.0f, 1.0f
    });

    constexpr auto m = []() {
        auto t = gleam::Transform2 {};
        t.Rotate(gleam::math::pi_over_2);
        t.Translate({0.0f, 1.0f});
        return t.Get();
    }();

    static_assert(m[0].x == c);
    static_assert(m[0].y == s);
    static_assert(m[1].x == -s);
    static_assert(m[1].y == c);
    static_assert(m[2].x == -1.0f);
    static_assert(m[2].y == 0.0f);
}

#pragma endregion