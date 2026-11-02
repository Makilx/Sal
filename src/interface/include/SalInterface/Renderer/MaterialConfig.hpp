#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Renderer/IShaders.hpp>
#include <SalInterface/Renderer/ITexture.hpp>

namespace sal::interface {
    struct MaterialConfig {
        IShaderProgram *Shaders = nullptr;
        ITexture *Texture = nullptr;
        Material *CustomMaterial = nullptr;

        static Material DEFAULT_MATERIAL;
    };

    inline Material MaterialConfig::DEFAULT_MATERIAL = {
        .Tint = Color::White,
        .Metallic = 0.0f,
        .Roughness = 1.0f,
    };
}
