#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Misc/Enums.hpp>
#include <SalInterface/Window/IWindow.hpp>
#include <SalInterface/Renderer/Material.hpp>
#include <SalInterface/Renderer/IMesh.hpp>

namespace sal::interface {
    struct RenderCommand {
        Material *Material = nullptr;
        IMesh *Mesh = nullptr;
        Vector<Matrix4> Transforms;
        bool Instanced = false;
    };

    struct RenderStatus {
        SizeT DrawCalls = 0;
        SizeT InstancedDrawCalls = 0;
    };

    struct RenderView {
        Matrix4 View;
        Matrix4 Projection;
    };

    struct RenderViewport {
        Vector2 Begin = Vector2::Zero;
        Vector2 Size = Vector2::One;
        bool Scale = true;
    };

    class IRenderer {
    public:
        IRenderer(RendererAPI api) : _renderAPI(api) {}
        virtual ~IRenderer() = default;

        RendererAPI GetRendererAPI() const { return _renderAPI; }

    public:
        virtual bool Initialize(IWindow *window) = 0;
        virtual bool IsInitialized() const = 0;
        virtual void Terminate() = 0;

        virtual void BeginFrame() = 0;
        virtual bool IsInFrame() const = 0;
        virtual void EndFrame() = 0;

        virtual IMesh *CreateMesh() = 0;
        virtual IShader *CreateShader() = 0;
        virtual ITexture *CreateTexture() = 0;
        virtual IShaderProgram *CreateProgram() = 0;

        virtual void Draw(const RenderCommand &command) = 0;

        virtual RenderStatus GetRenderStatus() const = 0;

        virtual void SetBackgroundColor(Color backgroundColor) = 0;
        virtual Color GetBackgroundColor() const = 0;

        virtual void SetRenderView(RenderView view) = 0;
        virtual RenderView GetRenderView() const = 0;

        virtual void SetRenderViewport(RenderViewport viewport) = 0;
        virtual RenderViewport GetRenderViewport() const = 0;

        virtual IWindow *GetCurrentWindow() const = 0;

    private:
        RendererAPI _renderAPI;
    };
}
