#pragma once

#include <SalCore/Core.hpp>

namespace sal {
    // Window
    struct WindowCreatedEvent final : public Event {
        String ToString() const override { return "Window Created"; }
    };

    struct WindowDestroyedEvent final : public Event {
        String ToString() const override { return "Window Destroyed"; }
    };

    struct WindowStateChangedEvent final : public Event {
        String ToString() const override { return sal::ToString(State); }
        WindowState State;
    };

    struct WindowResizedEvent final : public Event {
        String ToString() const override { return Size.ToString(); }
        IVector2 Size;
    };

    struct WindowMovedEvent final : public Event {
        String ToString() const override { return Position.ToString(); }
        IVector2 Position;
    };

    struct WindowVSYNCStateChangedEvent final : public Event {
        String ToString() const override { return sal::ToString(Enabled); }
        bool Enabled;
    };

    struct WindowTitleChangedEvent final : public Event {
        String ToString() const override { return Title; }
        String Title;
    };

    struct WindowShownEvent final : public Event {
        String ToString() const override { return "Window Shown"; }
    };

    struct WindowHiddenEvent final : public Event {
        String ToString() const override { return "Window Hidden"; }
    };

    struct WindowFocusedEvent final : public Event {
        String ToString() const override { return "Window Focused"; }
    };

    Sal_Stringify_Prebuilt(WindowShownEvent);
    Sal_Stringify_Prebuilt(WindowMovedEvent);
    Sal_Stringify_Prebuilt(WindowHiddenEvent);
    Sal_Stringify_Prebuilt(WindowFocusedEvent);
    Sal_Stringify_Prebuilt(WindowResizedEvent);
    Sal_Stringify_Prebuilt(WindowCreatedEvent);
    Sal_Stringify_Prebuilt(WindowDestroyedEvent);
    Sal_Stringify_Prebuilt(WindowTitleChangedEvent);
    Sal_Stringify_Prebuilt(WindowStateChangedEvent);
    Sal_Stringify_Prebuilt(WindowVSYNCStateChangedEvent);

    // Input
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

    // RunService
    struct RenderStepedEvent final : public Event {
        String ToString() const override { return sal::ToString(DeltaTime); }
        double DeltaTime;
    };

    Sal_Stringify_Prebuilt(RenderStepedEvent);
}
