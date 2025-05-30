// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#pragma once

#include "core/program_attributes.hpp"
#include "core/shader_library.hpp"
#include "renderer/gl/gl_program.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace engine {

class GLPrograms {
public:
    auto GetProgram(const ProgramAttributes& attrs) -> GLProgram*;

private:
    ShaderLibrary shader_lib_;

    std::unordered_map<std::string, std::unique_ptr<GLProgram>> programs_ {};
};

}