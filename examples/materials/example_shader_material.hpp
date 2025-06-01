// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#pragma once

#include <gleam/core.hpp>
#include <gleam/materials.hpp>
#include <gleam/nodes.hpp>

#include <memory>

#include "example_scene.hpp"

class ExampleShaderMaterial : public ExampleScene {
public:
    explicit ExampleShaderMaterial(std::shared_ptr<engine::Camera>);

    auto OnUpdate(float delta) -> void override;

    auto ContextMenu() -> void override;

private:
    engine::Timer timer_ {true};

    std::shared_ptr<engine::ShaderMaterial> material_;
    std::shared_ptr<engine::Mesh> mesh_;
};