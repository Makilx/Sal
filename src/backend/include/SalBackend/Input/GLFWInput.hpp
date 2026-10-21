#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Input/IInput.hpp>

namespace sal::backend {
    class GLFWInput final : public sal::interface::IInput {
    public:
        GLFWInput(interface::IWindow *window) : sal::interface::IInput(window) {
            _pressingKeys.fill(false);
            _previousPressingKeys.fill(false);
            _pressingMouseButtons.fill(false);
            _previousPressingMouseButtons.fill(false);
        }
        ~GLFWInput() override { Terminate(); }

        void Initialize() override;
        void Terminate() override;
        bool IsInitialized() const override { return _loaded; }

        bool GetInput(Key key) const override {
            return _pressingKeys[static_cast<SizeT>(key)];
        }

        bool GetInputUp(Key key) const override {
            return !_pressingKeys[static_cast<SizeT>(key)] &&
                   _previousPressingKeys[static_cast<SizeT>(key)];
        }

        bool GetInputDown(Key key) const override {
            return _pressingKeys[static_cast<SizeT>(key)] &&
                   !_previousPressingKeys[static_cast<SizeT>(key)];
        }

        bool GetInput(MouseButton button) const override {
            return _pressingMouseButtons[static_cast<SizeT>(button)];
        }

        bool GetInputUp(MouseButton button) const override {
            return !_pressingMouseButtons[static_cast<SizeT>(button)] &&
                   _previousPressingMouseButtons[static_cast<SizeT>(button)];
        }

        bool GetInputDown(MouseButton button) const override {
            return _pressingMouseButtons[static_cast<SizeT>(button)] &&
                   !_previousPressingMouseButtons[static_cast<SizeT>(button)];
        }

        void SimulateInput(MouseButton button, InputState state) override;
        void SimulateInput(Key key, InputState state) override;

        Vector2 GetMouseDelta() const override { return _mouseDelta; }
        Vector2 GetMousePosition() const override { return _mousePosition; }
        MouseState GetMouseState() const override { return _mouseState; }
        Vector2 GetMouseScroll() const override { return _mouseScroll; }

        void SetMouseState(MouseState state) override;
        void SetMousePosition(Vector2 position) override;

    public:
        void OnUpdate();

    private:
        bool _loaded = false;

        Vector2 _mousePosition = Vector2::Zero;
        Vector2 _lastMousePosition = Vector2::Zero;
        Vector2 _mouseDelta = Vector2::Zero;
        Vector2 _mouseScroll = Vector2::Zero;

        MouseState _mouseState = MouseState::Normal;

        Array<bool, 118> _pressingKeys;
        Array<bool, 118> _previousPressingKeys;

        Array<bool, 9> _pressingMouseButtons;
        Array<bool, 9> _previousPressingMouseButtons;
    };
}
