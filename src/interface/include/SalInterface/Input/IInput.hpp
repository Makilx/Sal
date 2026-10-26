#pragma once

#include <SalCore/Core.hpp>
#include <SalCore/Misc/Enums.hpp>
#include <SalCore/Misc/Events.hpp>

namespace sal::interface {
    class IWindow;

    class IInput {
    public:
        IInput(IWindow *window) : _window(window) {}
        virtual ~IInput() = default;

        IWindow &GetWindow() const { return *_window; }

    public:
        virtual void Terminate() = 0;
        virtual void Initialize() = 0;
        virtual bool IsInitialized() const = 0;

        virtual bool GetInput(Key key) const = 0;
        virtual bool GetInputUp(Key key) const = 0;
        virtual bool GetInputDown(Key key) const = 0;
        virtual bool GetInput(MouseButton button) const = 0;
        virtual bool GetInputUp(MouseButton button) const = 0;
        virtual bool GetInputDown(MouseButton button) const = 0;

        virtual void SimulateInput(MouseButton button, InputState state) = 0;
        virtual void SimulateInput(Key key, InputState state) = 0;

        virtual Vector2 GetMouseDelta() const = 0;
        virtual Vector2 GetMousePosition() const = 0;
        virtual MouseState GetMouseState() const = 0;
        virtual Vector2 GetMouseScroll() const = 0;

        virtual void SetMouseState(MouseState state) = 0;
        virtual void SetMousePosition(Vector2 position) = 0;

    public:
        Signal<InputStateEvent> InputBegin;
        Signal<InputStateEvent> InputChanged;
        Signal<InputStateEvent> InputEnded;
        Signal<MouseStateChangedEvent> MouseStateChanged;

    private:
        IWindow *_window;
    };
}
