// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#include "example_arbitrary_mesh.hpp"

#include "ui_helpers.hpp"

#include <engine/lights.hpp>
#include <engine/resources.hpp>

using namespace engine;
using namespace engine::math;

ExampleArbitraryMesh::ExampleArbitraryMesh(std::shared_ptr<engine::Camera> camera) {
    show_context_menu_ = false;

    Add(OrbitControls::Create(camera, {
        .radius = 4.0f,
        .pitch = DegToRad(5.0f),
        .yaw = DegToRad(15.0f)
    }));

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
    point_light->transform.Translate({2.0f, 2.0f, 4.0f});
    point_light->SetDebugMode(true);
    Add(point_light);
}

auto ExampleArbitraryMesh::OnAttached() -> void {
    this->Context()->Loaders().Mesh->LoadAsync(
        "assets/bunny.msh",
        [this](auto result) {
            if (result) {
                mesh_ = result.value();
                Add(mesh_);
            }
        }
    );
}

auto ExampleArbitraryMesh::ContextMenu() -> void {
    // Empty
}