// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#include "example_blending.hpp"

#include "ui_helpers.hpp"

#include <engine/geometries.hpp>
#include <engine/lights.hpp>
#include <engine/resources.hpp>

using namespace engine;

ExampleBlending::ExampleBlending(std::shared_ptr<engine::Camera> camera) {
    const auto camera_controls = OrbitControls::Create(camera, {.radius = 3.0f});
    Add(camera_controls);

    auto plane_geometry = PlaneGeometry::Create({.width = 2.0f, .height = 2.0f});
    auto opaque_material = PhongMaterial::Create();
    auto plane = Mesh::Create(plane_geometry, opaque_material);
    opaque_material->color = 0xE5BEED;
    opaque_material->two_sided = true;
    plane->TranslateZ(-0.5f);
    Add(plane);

    auto sphere_geometry = SphereGeometry::Create({.radius = 0.5f});
    transparent_material_ = PhongMaterial::Create();
    auto sphere = Mesh::Create(sphere_geometry, transparent_material_);
    transparent_material_->color = 0x049EF4;
    transparent_material_->transparent = true;
    transparent_material_->opacity = 0.9f;
    sphere->TranslateX(0.7f);
    Add(sphere);

    auto ambient_light = AmbientLight::Create(0xFFFFFF, 0.3f);
    Add(ambient_light);

    auto point_light = PointLight::Create(0xFFFFFF, 1.0f);
    point_light->transform.Translate({2.0f, 2.0f, 2.0f});
    Add(point_light);
}

auto ExampleBlending::ContextMenu() -> void {
    auto _ = true;
    static auto curr_blend_mode = std::string {"normal"};
    static auto blending_modes = std::array<const char*, 5> {
        "none", "normal", "additive", "subtractive", "multiply"
    };

    UIDropDown("mode", blending_modes, curr_blend_mode,
      [this](std::string_view str) {
        using enum Blending;
        curr_blend_mode = str;
        if (str == "none") transparent_material_->blending = None;
        if (str == "normal") transparent_material_->blending = Normal;
        if (str == "additive") transparent_material_->blending = Additive;
        if (str == "subtractive") transparent_material_->blending = Subtractive;
        if (str == "multiply") transparent_material_->blending = Multiply;
    });

    UISliderFloat("opacity", transparent_material_->opacity, 0.0f, 1.0f, _, 160.0f);
}