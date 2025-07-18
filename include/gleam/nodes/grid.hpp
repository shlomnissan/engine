/*
===========================================================================
  GLEAM ENGINE https://gleamengine.org
  Copyright © 2024 - Present, Shlomi Nissan
===========================================================================
*/

#pragma once

#include "gleam_export.h"

#include "gleam/math/color.hpp"
#include "gleam/nodes/node.hpp"

#include <memory>

namespace gleam {

/**
 * @brief Visual primitive representing a finite reference grid on the XZ plane.
 *
 * `Grid` is a scene node used to display a uniform grid, typically for
 * orientation and spatial reference in 3D scenes. It is useful during
 * development to visualize world units and object placement.
 *
 * The grid is centered at the origin and lies flat on the XZ plane.
 * Grid lines are evenly spaced based on the provided size and division count.
 *
 * @code
 * auto grid = gleam::Grid::Create({
 *     .size = 4.0f,
 *     .divisions = 16,
 *     .color = Color(0x333333)
 * });
 * scene->Add(grid);
 * @endcode
 *
 * @ingroup NodesGroup
 */
class GLEAM_EXPORT Grid : public Node {
public:
    /// @brief Parameters for constructing an Grid object.
    struct Parameters {
        float size; ///< Size of the grid.
        unsigned divisions; ///< Number of divisions.
        Color color; ///< Color of the grid.
    };

    /**
     * @brief Constructs a Grid object.
     *
     * @param params Grid::Parameters
     */
    explicit Grid(const Parameters& params);

    /**
     * @brief Creates a shared pointer to a Grid object.
     *
     * @param params Grid::Parameters
     * @return std::shared_ptr<Grid>
     */
    [[nodiscard]] static auto Create(const Parameters& params) {
        return std::make_shared<Grid>(params);
    }
};

}