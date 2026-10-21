#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Window/IWindow.hpp>
#include <SalBackend/Input/GLFWInput.hpp>

namespace sal::backend {
    class GLFWWindow final : public interface::IWindow {
    public:
        GLFWWindow()
            : interface::IWindow(WindowAPI::GLFW), _inputManager(this) {}
        ~GLFWWindow() override { Terminate(); }

    public:
        void Initialize() override;
        bool IsInitialized() const override { return _loaded; }
        void Terminate() override;

        void PollEvents() override;
        void SwapBuffers() override;

        void Show() override;
        void Hide() override;
        void Focus() override;

        bool IsHidden() const override;
        bool IsFocused() const override;
        IVector2 GetSize() const override;
        bool ShouldClose() const override;
        bool IsVSYNCEnabled() const override { return _vsyncEnabled; }
        WindowState GetState() const override { return _state; }
        IVector2 GetPosition() const override;
        interface::IInput &GetInputManager() const override {
            return (interface::IInput &)_inputManager;
        }

        void SetSize(IVector2 size) override;
        void SetVSYNC(bool enabled) override;
        void SetState(WindowState state) override;
        void SetTitle(const String &title) override;
        void SetPosition(IVector2 position) override;

    public:
        void *GetHandle() const { return _handle; }

    private:
        bool _loaded = false;
        void *_handle = nullptr;

        String _title = "";
        IVector2 _size = IVector2(500, 500);
        IVector2 _windowedSize = IVector2(500, 500);
        IVector2 _windowedPos = IVector2(0, 0);
        bool _vsyncEnabled = true;
        WindowState _state = WindowState::Windowed;

        GLFWInput _inputManager;
    };
}
