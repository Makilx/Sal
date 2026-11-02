#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Color.hpp>

namespace sal {
    struct Material {
        Color Tint = Color::White;
        float Metallic = 0.0f;
        float Roughness = 1.0f;
    };
}
