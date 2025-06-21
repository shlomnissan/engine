/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#include <memory>
#include <string_view>

#include <gleam/core.hpp>
#include <gleam/resources.hpp>
#include <gleam/math.hpp>

#include "ui_helpers.hpp"

#include "example_scene.hpp"
#include "examples.hpp"

#include <imgui.h>

using namespace gleam;

class Application : public ApplicationContextXYZ {
public:
    auto Configure() -> void override {
        params.antialiasing = 0;
        params.clear_color = 0x444444;
        params.debug = true;
        params.height = 1152;
        params.title = "Gleam Engine";
        params.vsync = false;
        params.width = 1536;
    }

    auto CreateScene() -> std::shared_ptr<Scene> override {
        LoadScene(examples[current_scene_]);
        Theme();
        return scene_;
    }

    auto CreateCamera() -> std::shared_ptr<Camera> override {
        camera_ = PerspectiveCamera::Create({
            .fov = math::DegToRad(60.0f),
            .aspect = params.Ratio(),
            .near = 0.1f,
            .far = 1000.0f
        });

        return camera_;
    }

    auto Update(float delta) -> bool override {
        const auto height = static_cast<float>(params.height);
        ImGui::SetNextWindowSize({250, height - 20.0f});
        ImGui::SetNextWindowPos({10, 10});
        ImGui::Begin("Gleam Engine", nullptr,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove
        );
        if (ImGui::CollapsingHeader("Examples", ImGuiTreeNodeFlags_DefaultOpen)) DrawExamplesList();

        if (scene_->show_context_menu_) {
            if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
                scene_->ContextMenu();
            }
        }

        ImGui::End();
        return true;
    }

    auto DrawExamplesList() -> void {
        if (ImGui::BeginListBox("##ListBox", {235, 260})) {
            for (auto i = 0; i < examples.size(); i++) {
                const auto name = std::string_view {examples[i]};
                if (name.empty()) {
                    ImGui::Separator();
                } else if (ImGui::Selectable(name.data(), current_scene_ == i) && current_scene_ != i) {
                    current_scene_ = i;
                    LoadScene(name);
                }
            }
            ImGui::EndListBox();
        }
    }

private:
    std::shared_ptr<PerspectiveCamera> camera_;
    std::shared_ptr<ExampleScene> scene_;

    int current_scene_ = 0;

    auto LoadScene(const std::string_view scene_name) -> void {
        if (scene_name == "Flat Material") {
            scene_ = std::make_shared<ExampleFlatMaterial>(camera_);
        }
        if (scene_name == "Phong Material") {
            scene_ = std::make_shared<ExamplePhongMaterial>(camera_);
        }
        if (scene_name == "Shader Material") {
            scene_ = std::make_shared<ExampleShaderMaterial>(camera_);
        }
        if (scene_name == "Directional Light") {
            scene_ = std::make_shared<ExampleDirectionalLight>(camera_);
        }
        if (scene_name == "Point Light") {
            scene_ = std::make_shared<ExamplePointLight>(camera_);
        }
        if (scene_name == "Spot Light") {
            scene_ = std::make_shared<ExampleSpotLight>(camera_);
        }
        if (scene_name == "Arbitrary Mesh") {
            scene_ = std::make_shared<ExampleArbitraryMesh>(camera_);
        }
        if (scene_name == "Primitive Mesh") {
            scene_ = std::make_shared<ExamplePrimitiveMesh>(camera_);
        }
        if (scene_name == "Blending Effect") {
            scene_ = std::make_shared<ExampleBlending>(camera_);
        }
        if (scene_name == "Fog Effect") {
            scene_ = std::make_shared<ExampleFog>(camera_);
        }
        if (scene_name == "Arrow Resource") {
            scene_ = std::make_shared<ExampleArrowResource>(camera_);
        }
        if (scene_name == "Grid Resource") {
            scene_ = std::make_shared<ExampleGridResource>(camera_);
        }
        if (scene_name == "Frustum Culling Test") {
            scene_ = std::make_shared<ExampleFrustumCullingTest>(camera_);
        }
        if (scene_name == "Lerp Animation Test") {
            scene_ = std::make_shared<ExampleLerpAnimationTest>(camera_);
        }

        SetScene(scene_);
    }
};

auto main() -> int {
    auto app = Application {};
    app.Start();

    return 0;
}