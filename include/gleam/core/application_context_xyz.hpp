/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#pragma once

#include "gleam_export.h"

#include "gleam/cameras/camera.hpp"
#include "gleam/core/shared_context.hpp"
#include "gleam/core/timer.hpp"
#include "gleam/math/color.hpp"
#include "gleam/nodes/scene.hpp"

#include <memory>
#include <string>

namespace gleam {

class GLEAM_EXPORT ApplicationContextXYZ {
public:
    struct Parameters {
        std::string title {"Gleam Application"};
        Color clear_color {0x000000};
        int width {1024};
        int height {768};
        int antialiasing {0};
        bool vsync {true};
        bool debug {false};

        [[nodiscard]] auto Ratio() const -> float {
            return static_cast<float>(width) / static_cast<float>(height);
        }
    };

    ApplicationContextXYZ();

    auto Start() -> void;

    virtual auto Configure() -> void {};

    virtual auto CreateScene() -> std::shared_ptr<Scene> = 0;

    virtual auto CreateCamera() -> std::shared_ptr<Camera> { return nullptr; }

    virtual auto Update(float delta) -> bool = 0;

    [[nodiscard]] auto GetScene() const -> const Scene*;

    [[nodiscard]] auto GetCamera() const -> const Camera*;

    auto SetScene(std::shared_ptr<Scene> scene) -> void;

    auto SetCamera(std::shared_ptr<Camera> camera) -> void;

    virtual ~ApplicationContextXYZ();

protected:
    Parameters params;

    Timer timer {false};

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    auto Setup() -> void;

    auto CreateDefaultCamera() const -> std::shared_ptr<Camera>;
};

}