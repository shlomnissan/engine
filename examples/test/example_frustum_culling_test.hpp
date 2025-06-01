// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#pragma once

#include <gleam/core.hpp>
#include <gleam/math.hpp>
#include <gleam/nodes.hpp>

#include "example_scene.hpp"

#include <array>

class ExampleFrustumCullingTest : public ExampleScene {
public:
    explicit ExampleFrustumCullingTest(std::shared_ptr<gleam::Camera>);

    auto ContextMenu() -> void override;

    auto OnUpdate(float delta) -> void override;

private:
    gleam::Frustum frustum_;

    std::array<std::shared_ptr<gleam::Mesh>, 2500> boxes_;
};