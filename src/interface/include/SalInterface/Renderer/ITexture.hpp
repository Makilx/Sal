#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Misc/Enums.hpp>
#include <SalInterface/Renderer/IRenderResource.hpp>

namespace sal::interface {
    class ITexture : public IRenderResource {
    public:
        ITexture(RendererAPI api) : _renderAPI(api) {}
        virtual ~ITexture() = default;

        RendererAPI GetRendererAPI() const { return _renderAPI; }

    public:
        virtual bool Create(const Path &file,
                            TextureType type = TextureType::Texture2D) = 0;
        virtual bool Create(TextureImageFormat format, const IVector2 &size,
                            const Vector<Byte> &data,
                            TextureType type = TextureType::Texture2D) = 0;
        virtual bool IsCreated() const = 0;
        virtual void Destroy() = 0;

        virtual TextureImageFormat GetImageFormat() const = 0;
        virtual TextureType GetType() const = 0;
        virtual TextureWrap GetWrap(TextureWrapChannel channel) const = 0;
        virtual TextureFilter GetFilter(TextureFilterChannel channel) const = 0;
        virtual IVector2 GetSize() const = 0;

        virtual void SetWrap(TextureWrapChannel channel, TextureWrap wrap) = 0;
        virtual void SetFilter(TextureFilterChannel channel,
                               TextureFilter filter) = 0;

    private:
        RendererAPI _renderAPI;
    };
}
