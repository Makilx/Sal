#pragma once

#include <SalCore/Core.hpp>
#include <SalCore/Misc/Enums.hpp>
#include <SalInterface/Renderer/IRenderResource.hpp>

namespace sal::interface {
    class IShader : public IRenderResource {
    public:
        IShader(RendererAPI api) : _renderAPI(api) {}
        virtual ~IShader() = default;

        RendererAPI GetRendererAPI() const { return _renderAPI; }

    public:
        virtual bool Create(ShaderType type, const String &source) = 0;
        virtual bool IsCreated() const = 0;
        virtual void Destroy() = 0;

        virtual ShaderType GetShaderType() const = 0;

    private:
        RendererAPI _renderAPI;
    };

    class IShaderProgram : public IRenderResource {
    public:
        IShaderProgram(RendererAPI api) : _renderAPI(api) {}
        virtual ~IShaderProgram() = default;

        RendererAPI GetRendererAPI() const { return _renderAPI; }

    public:
        virtual bool Create() = 0;
        virtual bool IsCreated() const = 0;
        virtual void Destroy() = 0;

        virtual void Attach(const IShader &shader) = 0;
        virtual void Link() = 0;

        virtual void Set(const String &name, int value) = 0;
        virtual void Set(const String &name, float value) = 0;
        virtual void Set(const String &name, const Vector2 &value) = 0;
        virtual void Set(const String &name, const Vector3 &value) = 0;
        virtual void Set(const String &name, const Vector4 &value) = 0;
        virtual void Set(const String &name, const Color &value) = 0;
        virtual void Set(const String &name, const Matrix4 &value) = 0;
        virtual void Set(const String &name, const CFrame &value) = 0;

    private:
        RendererAPI _renderAPI;
    };
}
