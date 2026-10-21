#include <SalBackend/Window/GLFWWindow.hpp>
#include <GLFW/glfw3.h>

namespace {
    static sal::SizeT INSTANCE_COUNT = 0;
}

namespace sal::backend {
    void GLFWWindow::Initialize() {
        if (_loaded) {
            Terminate();
        }

        // window setup
        if (INSTANCE_COUNT == 0) {
            if (!glfwInit()) {
                Console::Error("Window => Failed to initialize GLFW");
                return;
            }
        }

        INSTANCE_COUNT++;

        _handle =
            glfwCreateWindow(_size.X, _size.Y, _title.c_str(), NULL, NULL);

        if (!_handle) {
            Console::Error("Window => Failed to create GLFW window");
            return;
        }

        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwMakeContextCurrent(windowHandle);
        glfwSetWindowUserPointer(windowHandle, this);

        // window signals
        glfwSetWindowSizeCallback(
            windowHandle, [](GLFWwindow *window, int width, int height) {
                GLFWWindow *self =
                    static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));

                if (!self) {
                    return;
                }

                self->_size = IVector2(width, height);

                WindowResizedEvent event;
                event.Size = self->_size;
                self->Resized.Fire(event);
            });

        glfwSetWindowPosCallback(
            windowHandle, [](GLFWwindow *window, int x, int y) {
                GLFWWindow *self =
                    static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));

                if (!self) {
                    return;
                }

                WindowMovedEvent event;
                event.Position = IVector2(x, y);
                self->Moved.Fire(event);
            });

        glfwSetWindowFocusCallback(
            windowHandle, [](GLFWwindow *window, int focused) {
                if (!focused) {
                    return;
                }

                GLFWWindow *self =
                    static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));

                if (!self) {
                    return;
                }

                WindowFocusedEvent event;
                self->Focused.Fire(event);
            });

        // window defaults
        _loaded = true;
        SetVSYNC(_vsyncEnabled);
        SetState(_state);
        glfwGetWindowPos(windowHandle, &_windowedPos.X, &_windowedPos.Y);
        glfwGetWindowSize(windowHandle, &_windowedSize.X, &_windowedSize.Y);

        // initialize input
        _inputManager.Initialize();

        WindowCreatedEvent event;
        Created.Fire(event);
    }

    void GLFWWindow::Terminate() {
        if (!_loaded) {
            return;
        }

        WindowDestroyedEvent event;
        Destroyed.Fire(event);

        _inputManager.Terminate();

        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwDestroyWindow(windowHandle);
        _handle = nullptr;

        _loaded = false;
        INSTANCE_COUNT--;
        if (INSTANCE_COUNT == 0) {
            glfwTerminate();
        }
    }

    void GLFWWindow::PollEvents() {
        if (!_loaded) {
            return;
        }
        glfwPollEvents();
    }

    void GLFWWindow::SwapBuffers() {
        if (!_loaded) {
            return;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwSwapBuffers(windowHandle);
    }

    void GLFWWindow::Show() {
        if (!_loaded) {
            return;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwShowWindow(windowHandle);

        WindowShownEvent event;
        Shown.Fire(event);
    }

    void GLFWWindow::Hide() {
        if (!_loaded) {
            return;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwHideWindow(windowHandle);

        WindowHiddenEvent event;
        Hidden.Fire(event);
    }

    void GLFWWindow::Focus() {
        if (!_loaded) {
            return;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwFocusWindow(windowHandle);
    }

    bool GLFWWindow::IsHidden() const {
        if (!_loaded) {
            return false;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        return glfwGetWindowAttrib(windowHandle, GLFW_VISIBLE) == GLFW_FALSE;
    }

    bool GLFWWindow::IsFocused() const {
        if (!_loaded) {
            return false;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        return glfwGetWindowAttrib(windowHandle, GLFW_FOCUSED) == GLFW_TRUE;
    }

    IVector2 GLFWWindow::GetSize() const {
        if (!_loaded) {
            return _size;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        int width, height;
        glfwGetWindowSize(windowHandle, &width, &height);
        return IVector2(width, height);
    }

    bool GLFWWindow::ShouldClose() const {
        if (!_loaded) {
            return true;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        return glfwWindowShouldClose(windowHandle);
    }

    IVector2 GLFWWindow::GetPosition() const {
        if (!_loaded) {
            return _size;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        int xPos, yPos;
        glfwGetWindowPos(windowHandle, &xPos, &yPos);
        return IVector2(xPos, yPos);
    }

    void GLFWWindow::SetSize(IVector2 size) {
        _size = size;

        if (!_loaded || _state != WindowState::Windowed) {
            WindowResizedEvent event;
            event.Size = _size;
            Resized.Fire(event);
            return;
        }

        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwSetWindowSize(windowHandle, size.X, size.Y);
    }

    void GLFWWindow::SetVSYNC(bool enabled) {
        bool changed = _vsyncEnabled != enabled;
        _vsyncEnabled = enabled;

        if (changed) {
            WindowVSYNCStateChangedEvent event;
            event.Enabled = enabled;
            VSYNCStateChanged.Fire(event);
        }

        if (!_loaded) {
            return;
        }

        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void GLFWWindow::SetState(WindowState state) {
        const WindowState previousState = _state;
        const bool changed = previousState != state;
        _state = state;

        if (changed) {
            WindowStateChangedEvent event;
            event.State = state;
            StateChanged.Fire(event);
        }

        if (!_loaded) {
            _state = state;
            return;
        }

        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);

        if (previousState == WindowState::Windowed) {
            glfwGetWindowPos(windowHandle, &_windowedPos.X, &_windowedPos.Y);
            glfwGetWindowSize(windowHandle, &_windowedSize.X, &_windowedSize.Y);
        }

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();

        if (!monitor) {
            return;
        }

        const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

        if (!videoMode) {
            return;
        }

        switch (state) {
        case WindowState::Windowed:
            glfwSetWindowAttrib(windowHandle, GLFW_DECORATED, GLFW_TRUE);
            glfwRestoreWindow(windowHandle);
            glfwSetWindowMonitor(windowHandle, nullptr, _windowedPos.X,
                                 _windowedPos.Y, _windowedSize.X,
                                 _windowedSize.Y, GLFW_DONT_CARE);

            break;

        case WindowState::Fullscreen:
            glfwSetWindowAttrib(windowHandle, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowMonitor(windowHandle, monitor, 0, 0, videoMode->width,
                                 videoMode->height, videoMode->refreshRate);
            break;

        case WindowState::BorderlessFullscreen:
            glfwMaximizeWindow(windowHandle);
            glfwSetWindowAttrib(windowHandle, GLFW_DECORATED, GLFW_FALSE);
            break;
        }
    }

    void GLFWWindow::SetTitle(const String &title) {
        _title = title;
        WindowTitleChangedEvent event;
        event.Title = _title;
        TitleChanged.Fire(event);
        if (!_loaded) {
            return;
        }
        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwSetWindowTitle(windowHandle, title.c_str());
    }

    void GLFWWindow::SetPosition(IVector2 position) {
        if (!_loaded || _state != WindowState::Windowed) {
            return;
        }

        GLFWwindow *windowHandle = static_cast<GLFWwindow *>(_handle);
        glfwSetWindowPos(windowHandle, position.X, position.Y);
    }
}
