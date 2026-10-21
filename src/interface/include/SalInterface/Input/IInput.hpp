#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Misc/Enums.hpp>

namespace sal {
    struct InputStateEvent final : public Event {
        String ToString() const override { return sal::ToString(Type); }
        InputType Type = InputType::Unknown;
        InputState State = InputState::Begin;
        Key Key = Key::Unknown;
        MouseButton MouseButton = MouseButton::Unknown;
        Vector2 Position = Vector2::Zero;
        Vector2 Delta = Vector2::Zero;
    };

    struct MouseStateChangedEvent final : public Event {
        String ToString() const override { return sal::ToString(State); }
        MouseState State;
    };

    Sal_Stringify_Prebuilt(InputStateEvent);
    Sal_Stringify_Prebuilt(MouseStateChangedEvent);
}

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
