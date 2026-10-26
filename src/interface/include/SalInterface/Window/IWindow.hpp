#pragma once

#include <SalCore/Core.hpp>
#include <SalCore/Misc/Enums.hpp>
#include <SalCore/Misc/Events.hpp>
#include <SalInterface/Input/IInput.hpp>

namespace sal::interface {
    class IWindow {
    public:
        IWindow(WindowAPI api) : _windowAPI(api) {}
        virtual ~IWindow() = default;

        WindowAPI GetWindowAPI() const { return _windowAPI; }

    public:
        virtual bool Initialize() = 0;
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
        virtual String GetTitle() const = 0;

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
