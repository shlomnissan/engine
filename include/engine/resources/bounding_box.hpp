// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#pragma once

#include "engine_export.h"
#include "engine/core/geometry.hpp"
#include "engine/math/box3.hpp"
#include "engine/math/color.hpp"
#include "engine/nodes/node.hpp"

#include <memory>

namespace engine {

/**
 * @brief Generates geometry to visualize an engine::Box3 object.
 */
class ENGINE_EXPORT BoundingBox : public Node {
public:
    /**
     * @brief Constructs a BoundingBox object with a box and color.
     *
     * @param box The box to draw.
     * @param color The color of the bounding box.
     */
    BoundingBox(const Box3& box, const Color& color);

    /**
     * @brief Creates a new instance of the BoundingBox class.
     *
     * @param box The box to draw.
     * @param color The color of the bounding box.
     * @return A `std::shared_ptr<BoundingBox>` pointing to the newly created instance.
     */
    [[nodiscard]] static auto Create(const Box3& box, const Color& color) {
        return std::make_shared<BoundingBox>(box, color);
    }

private:
    /**
     * @brief Creates the geometry for the bounding box.
     *
     * @param box The box to draw.
     * @return A shared pointer to the created geometry.
     */
    auto CreateGeometry(const Box3& box) const -> std::shared_ptr<Geometry>;
};

}