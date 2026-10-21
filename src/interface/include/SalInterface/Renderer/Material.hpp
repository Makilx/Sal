#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Renderer/IShaders.hpp>
#include <SalInterface/Renderer/ITexture.hpp>

namespace sal::interface {
    class Material {
    public:
        IShaderProgram *Shaders = nullptr;
        ITexture *Texture = nullptr;

        Color Tint = Color::White;
    };
}
