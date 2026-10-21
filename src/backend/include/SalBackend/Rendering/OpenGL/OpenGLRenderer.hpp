#pragma once

#include <SalInterface/Renderer/IRenderer.hpp>

namespace sal::backend {
    class OpenGLRenderer final : public interface::IRenderer {
    public:
        OpenGLRenderer() : interface::IRenderer(RendererAPI::OpenGL) {}
        ~OpenGLRenderer() override { Terminate(); }

        bool Initialize(interface::IWindow *window) override;
        bool IsInitialized() const override { return _loaded; }
        void Terminate() override;

        void BeginFrame() override;
        bool IsInFrame() const override { return _inFrame; }
        void EndFrame() override;

        interface::IMesh *CreateMesh() override;
        interface::IShader *CreateShader() override;
        interface::ITexture *CreateTexture() override;
        interface::IShaderProgram *CreateProgram() override;

        void Draw(const interface::RenderCommand &command) override;

        interface::RenderStatus GetRenderStatus() const override {
            return _renderStatus;
        }

        void SetBackgroundColor(Color backgroundColor) override {
            _backgroundColor = backgroundColor;
        }
        Color GetBackgroundColor() const override { return _backgroundColor; }

        void SetRenderView(interface::RenderView view) override {
            _renderView = view;
        }
        interface::RenderView GetRenderView() const override {
            return _renderView;
        }

        void SetRenderViewport(interface::RenderViewport viewport) override {
            _viewport = viewport;
        }
        interface::RenderViewport GetRenderViewport() const override {
            return _viewport;
        }

        interface::IWindow *GetCurrentWindow() const override {
            return _currentWindow;
        }

    private:
        // state
        bool _loaded = false;
        interface::IWindow *_currentWindow = nullptr;

        // rendering
        bool _inFrame = false;
        Vector<interface::RenderCommand> _frameCommands;
        interface::RenderStatus _renderStatus;

        // settings
        Color _backgroundColor = Color::White;
        interface::RenderView _renderView;
        interface::RenderViewport _viewport;

        // resources
        Vector<UniquePtr<interface::IRenderResource>> _resources;

        // connections
        Connection<WindowResizedEvent> _resizedConnection;
    };
}
