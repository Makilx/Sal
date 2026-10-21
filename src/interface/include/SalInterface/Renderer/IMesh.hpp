#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Misc/Enums.hpp>
#include <SalInterface/Renderer/IRenderResource.hpp>

namespace sal::interface {
    struct Vertex {
        Vector3 Position;
        Vector3 Normal;
        Vector2 TextureUV;
    };

    class IMesh : public IRenderResource {
    public:
        IMesh(RendererAPI api) : _renderAPI(api) {}
        virtual ~IMesh() = default;

        RendererAPI GetRendererAPI() const { return _renderAPI; }

    public:
        virtual bool Create() = 0;
        virtual bool IsCreated() const = 0;
        virtual void Destroy() = 0;

        virtual void SetVertices(const Vector<Vertex> &vertices) = 0;
        virtual void SetIndices(const Vector<unsigned int> &indices) = 0;

        virtual SizeT GetIndicesCount() const = 0;

    private:
        RendererAPI _renderAPI;
    };
}
