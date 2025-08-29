/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#pragma once

#include "gleam/lights/light.hpp"
#include "gleam/nodes/node.hpp"
#include "gleam/nodes/renderable.hpp"
#include "gleam/nodes/scene.hpp"

#include <memory>
#include <span>
#include <vector>

namespace gleam {

class RenderLists {
public:
    auto ProcessScene(Scene* scene) -> void;

    [[nodiscard]] auto Opaque() const -> std::span<Renderable* const> {
        return opaque_;
    }

    [[nodiscard]] auto Transparent() const -> std::span<Renderable* const> {
        return transparent_;
    }

    [[nodiscard]] auto Lights() const -> std::span<Light* const> {
        return lights_;
    }

private:
    std::vector<Renderable*> opaque_;

    std::vector<Renderable*> transparent_;

    std::vector<Light*> lights_;

    auto ProcessNode(Node*) -> void;

    auto Reset() -> void;
};

}