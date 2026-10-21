#pragma once

#include <SalInterface/Renderer/ITexture.hpp>

namespace sal::backend {
    class OpenGLTexture final : public interface::ITexture {
    public:
        OpenGLTexture() : interface::ITexture(RendererAPI::OpenGL) {}
        ~OpenGLTexture() override { Destroy(); }

        bool Create(const Path &file,
                    TextureType type = TextureType::Texture2D) override;
        bool Create(TextureImageFormat format, const IVector2 &size,
                    const Vector<Byte> &data,
                    TextureType type = TextureType::Texture2D) override;
        bool IsCreated() const override { return _created; }
        void Destroy() override;

        TextureImageFormat GetImageFormat() const override { return _format; }
        TextureType GetType() const override { return _type; }
        TextureWrap GetWrap(TextureWrapChannel channel) const override {
            return channel == TextureWrapChannel::S ? _wrapS : _wrapT;
        }
        TextureFilter GetFilter(TextureFilterChannel channel) const override {
            return channel == TextureFilterChannel::MagFilter ? _magFilter
                                                              : _minFilter;
        }
        IVector2 GetSize() const override { return _size; }

        void SetWrap(TextureWrapChannel channel, TextureWrap wrap) override;
        void SetFilter(TextureFilterChannel channel,
                       TextureFilter filter) override;

    public:
        unsigned int GetHandle() const { return _texture; }

    private:
        bool _created = false;
        unsigned int _texture = 0;

        TextureType _type = TextureType::Texture2D;
        TextureImageFormat _format = TextureImageFormat::RGB;
        TextureFilter _magFilter = TextureFilter::Linear;
        TextureFilter _minFilter = TextureFilter::Linear;
        TextureWrap _wrapS = TextureWrap::Repeat;
        TextureWrap _wrapT = TextureWrap::Repeat;
        IVector2 _size = IVector2::Zero;
    };
}
