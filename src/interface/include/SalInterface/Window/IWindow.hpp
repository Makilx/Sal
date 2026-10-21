#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Misc/Enums.hpp>
#include <SalInterface/Input/IInput.hpp>

namespace sal {
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
}

namespace sal::interface {
    class IWindow {
    public:
        IWindow(WindowAPI api) : _windowAPI(api) {}
        virtual ~IWindow() = default;

        WindowAPI GetWindowAPI() const { return _windowAPI; }

    public:
        virtual void Initialize() = 0;
        virtual void Terminate() = 0;
        virtual bool IsInitialized() const = 0;

        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;

        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Focus() = 0;

        virtual bool IsHidden() const = 0;
        virtual bool IsFocused() const = 0;
        virtual IVector2 GetSize() const = 0;
        virtual bool ShouldClose() const = 0;
        virtual bool IsVSYNCEnabled() const = 0;
        virtual WindowState GetState() const = 0;
        virtual IVector2 GetPosition() const = 0;
        virtual IInput &GetInputManager() const = 0;

        virtual void SetSize(IVector2 size) = 0;
        virtual void SetVSYNC(bool enabled) = 0;
        virtual void SetState(WindowState state) = 0;
        virtual void SetTitle(const String &title) = 0;
        virtual void SetPosition(IVector2 position) = 0;

    public:
        Signal<WindowCreatedEvent> Created;
        Signal<WindowDestroyedEvent> Destroyed;
        Signal<WindowMovedEvent> Moved;
        Signal<WindowResizedEvent> Resized;
        Signal<WindowStateChangedEvent> StateChanged;
        Signal<WindowVSYNCStateChangedEvent> VSYNCStateChanged;
        Signal<WindowTitleChangedEvent> TitleChanged;
        Signal<WindowShownEvent> Shown;
        Signal<WindowHiddenEvent> Hidden;
        Signal<WindowFocusedEvent> Focused;

    private:
        WindowAPI _windowAPI;
    };
}
