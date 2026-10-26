#include <SalEngine/Services/WindowService.hpp>
#include <SalInterface/Window/IWindow.hpp>
#include <SalBackend/BackendFactory.hpp>
#include <SalEngine/DataModel/DataModel.hpp>
#include <SalEngine/Services/RunService.hpp>

namespace sal {
    WindowService::WindowService(DataModel &dataModel, void *handle)
        : Service(dataModel), _windowHandle(handle) {}

    WindowService::~WindowService() { Terminate(); }

    // ---------------------------- //

    String WindowService::GetWindowTitle() const {
        return static_cast<interface::IWindow *>(_windowHandle)->GetTitle();
    }

    IVector2 WindowService::GetWindowSize() const {
        return static_cast<interface::IWindow *>(_windowHandle)->GetSize();
    }

    IVector2 WindowService::GetWindowPos() const {
        return static_cast<interface::IWindow *>(_windowHandle)->GetPosition();
    }

    WindowState WindowService::GetWindowState() const {
        return static_cast<interface::IWindow *>(_windowHandle)->GetState();
    }

    bool WindowService::IsHidden() const {
        return static_cast<interface::IWindow *>(_windowHandle)->IsHidden();
    }

    // ---------------------------- //

    void WindowService::SetWindowSize(const IVector2 &size) {
        static_cast<interface::IWindow *>(_windowHandle)->SetSize(size);
    }

    void WindowService::SetWindowTitle(const String &title) {
        static_cast<interface::IWindow *>(_windowHandle)->SetTitle(title);
    }

    void WindowService::SetWindowPos(const IVector2 &pos) {
        static_cast<interface::IWindow *>(_windowHandle)->SetPosition(pos);
    }

    void WindowService::SetWindowState(WindowState state) {
        static_cast<interface::IWindow *>(_windowHandle)->SetState(state);
    }

    void WindowService::Focus() {
        static_cast<interface::IWindow *>(_windowHandle)->Focus();
    }

    void WindowService::Show() {
        static_cast<interface::IWindow *>(_windowHandle)->Show();
    }

    void WindowService::Hide() {
        static_cast<interface::IWindow *>(_windowHandle)->Hide();
    }

    // ---------------------------- //

    bool WindowService::Initialize() {
        bool success =
            static_cast<interface::IWindow *>(_windowHandle)->Initialize();

        if (success) {
            _loaded = true;

            // setup runservice updates
            RunService *runService = GetDataModel().GetService<RunService>();

            auto a = runService->BindToUpdate(
                static_cast<SizeT>(RunPriority::Input),
                [this](double deltaTime) {
                    static_cast<interface::IWindow *>(_windowHandle)
                        ->PollEvents();
                });

            auto b = runService->BindToUpdate(
                static_cast<SizeT>(RunPriority::Render),
                [this](double deltaTime) {
                    static_cast<interface::IWindow *>(_windowHandle)
                        ->SwapBuffers();
                });
        }

        return success;
    }

    void WindowService::Terminate() {
        _loaded = false;
        _windowHandle = nullptr;
    }
}
