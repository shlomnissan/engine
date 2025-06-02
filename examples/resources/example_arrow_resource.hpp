/*
===========================================================================
GLEAM ENGINE https://gleamengine.org
Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#pragma once

#include <gleam/nodes.hpp>

#include <memory>

#include "example_scene.hpp"

class ExampleArrowResource : public ExampleScene {
public:
    explicit ExampleArrowResource(std::shared_ptr<gleam::Camera>);

    auto ContextMenu() -> void override;
};