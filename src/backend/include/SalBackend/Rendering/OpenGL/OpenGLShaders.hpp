#pragma once

#include <SalInterface/Renderer/IShaders.hpp>

namespace sal::backend {
    class OpenGLShader final : public interface::IShader {
    public:
        OpenGLShader() : interface::IShader(RendererAPI::OpenGL) {}
        ~OpenGLShader() override { Destroy(); }

        bool Create(ShaderType type, const String &source) override;
        bool IsCreated() const override { return _created; }
        void Destroy() override;

        ShaderType GetShaderType() const override { return _type; }

    public:
        unsigned int GetHandle() const { return _shader; }

    private:
        bool _created = false;
        ShaderType _type = ShaderType::VertexShader;

        unsigned int _shader = 0;
    };

    class OpenGLShaderProgram final : public interface::IShaderProgram {
    public:
        OpenGLShaderProgram()
            : interface::IShaderProgram(RendererAPI::OpenGL) {}
        ~OpenGLShaderProgram() override { Destroy(); }

        bool Create() override;
        bool IsCreated() const override { return _created; }
        void Destroy() override;

        void Attach(const interface::IShader &shader) override;
        void Link() override;

        void Set(const String &name, int value) override;
        void Set(const String &name, float value) override;
        void Set(const String &name, const Vector2 &value) override;
        void Set(const String &name, const Vector3 &value) override;
        void Set(const String &name, const Vector4 &value) override;
        void Set(const String &name, const Color &value) override;
        void Set(const String &name, const CFrame &value) override;
        void Set(const String &name, const Matrix4 &value) override;

    public:
        unsigned int GetHandle() const { return _program; }

    private:
        int GetUniformLocation(const String &uniform);

    private:
        bool _created = false;
        unsigned int _program = 0;
        Dictionary<String, unsigned int> _uniformCache;
    };
}
