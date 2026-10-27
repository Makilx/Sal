#pragma once

#include <SalEngine/Services/Service.hpp>

namespace sal {
    class WindowService final : public Service {
    public:
        WindowService(DataModel &dataModel, void *windowHandle);

        bool IsInitialized() const override { return _loaded; }
        String ToString() const override { return "WindowService"; }

        WindowAPI GetWindowAPI() const;

        String GetWindowTitle() const;
        void SetWindowTitle(const String &title);

        IVector2 GetWindowSize() const;
        void SetWindowSize(const IVector2 &size);

        IVector2 GetWindowPos() const;
        void SetWindowPos(const IVector2 &pos);

        WindowState GetWindowState() const;
        void SetWindowState(WindowState state);

        bool IsHidden() const;

        void Focus();
        void Show();
        void Hide();

    public:
        Signal<WindowMovedEvent> Moved;
        Signal<WindowResizedEvent> Resized;
        Signal<WindowStateChangedEvent> StateChanged;
        Signal<WindowVSYNCStateChangedEvent> VSYNCStateChanged;
        Signal<WindowTitleChangedEvent> TitleChanged;
        Signal<WindowShownEvent> Shown;
        Signal<WindowHiddenEvent> Hidden;
        Signal<WindowFocusedEvent> Focused;

    private:
        bool Initialize() override;
        void Terminate() override;

    private:
        bool _loaded = false;
        void *_windowHandle = nullptr;

        friend class DataModel;
    };
}
