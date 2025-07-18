/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#include "example_phong_material.hpp"

#include "ui_helpers.hpp"

#include <gleam/geometries.hpp>
#include <gleam/lights.hpp>
#include <gleam/loaders.hpp>
#include <gleam/textures.hpp>

using namespace gleam;

ExamplePhongMaterial::ExamplePhongMaterial() {


    auto geometry = CubeGeometry::Create();
    material_ = PhongMaterial::Create(0x049EF4);
    material_->specular = {0.3f, 0.3, 0.3f};
    material_->shininess = 32.0f;
    mesh_ = Mesh::Create(geometry, material_);
    Add(mesh_);

    Add(AmbientLight::Create({
        .color = 0xFFFFFF,
        .intensity = 0.3f
    }));

    auto point_light = PointLight::Create({
        .color = 0xFFFFFF,
        .intensity = 1.0f,
        .attenuation = {
            .base = 1.0f,
            .linear = 0.0f,
            .quadratic = 0.0f
        }
    });
    point_light->transform.Translate({2.0f, 2.0f, 2.0f});
    Add(point_light);
}

auto ExamplePhongMaterial::OnAttached(gleam::SharedContext* context) -> void {
    Add(OrbitControls::Create(context->Parameters().camera, {.radius = 3.0f}));

    context->Loaders().Texture->LoadAsync(
        "assets/checker.tex",
        [this](auto result) {
            if (result) texture_ = result.value();
        }
    );
}

auto ExamplePhongMaterial::OnUpdate(float delta) -> void {
    mesh_->transform.Rotate(Vector3::Up(), 1.0f * delta);
    mesh_->transform.Rotate(Vector3::Right(), 1.0f * delta);
}

auto ExamplePhongMaterial::ContextMenu() -> void {
    auto _ = false;
    static auto curr_texture = std::string {"none"};
    static auto textures = std::array<const char*, 2> {
        "none", "checkerboard"
    };

    UIColor("color", &material_->color[0], _);
    UIColor("specular", &material_->specular[0], _);
    UISliderFloat("shininess", material_->shininess, 0.0f, 128.0f, _, 160.0f);
    UIDropDown("texture", textures, curr_texture,
      [this](std::string_view str) {
        curr_texture = str;
        if (str == "none") material_->texture_map = nullptr;
        if (str == "checkerboard") material_->texture_map = texture_;
    });

    UISeparator();

    UICheckbox("transparent", material_->transparent, _);
    UISliderFloat("opacity", material_->opacity, 0.0f, 1.0f, _, 160.0f);

    UISeparator();

    UICheckbox("depth_test", material_->depth_test, _);
    UICheckbox("flat_shaded", material_->flat_shaded, _);
    UICheckbox("fog", material_->fog, _);
    UICheckbox("two_sided", material_->two_sided, _);
    UICheckbox("wireframe", material_->wireframe, _);
}