// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#pragma once

#include "engine/math/matrix4.hpp"
#include "engine/nodes/node.hpp"

namespace engine {

/**
 * @brief An abstract base class for cameras.
 */
class Camera : public Node {
public:
    /// @brief The projection matrix of the camera.
    Matrix4 projection_transform;

    /// @brief The view matrix of the camera.
    Matrix4 view_transform;

    /**
     * @brief Updates the view matrix of the camera by the
     * inverse of the world transformation.
     */
    auto UpdateViewTransform() -> void;

private:
    /**
     * @brief Sets the projection matrix of the camera.
     */
    virtual auto SetProjection() -> void = 0;
};

}