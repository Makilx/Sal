#include <SalBackend/Input/GLFWInput.hpp>
#include <SalBackend/Window/GLFWWindow.hpp>
#include <GLFW/glfw3.h>

namespace sal {
    constexpr Key FromGLFWKey(int key) {
        switch (key) {
        case GLFW_KEY_A:
            return Key::A;
        case GLFW_KEY_B:
            return Key::B;
        case GLFW_KEY_C:
            return Key::C;
        case GLFW_KEY_D:
            return Key::D;
        case GLFW_KEY_E:
            return Key::E;
        case GLFW_KEY_F:
            return Key::F;
        case GLFW_KEY_G:
            return Key::G;
        case GLFW_KEY_H:
            return Key::H;
        case GLFW_KEY_I:
            return Key::I;
        case GLFW_KEY_J:
            return Key::J;
        case GLFW_KEY_K:
            return Key::K;
        case GLFW_KEY_L:
            return Key::L;
        case GLFW_KEY_M:
            return Key::M;
        case GLFW_KEY_N:
            return Key::N;
        case GLFW_KEY_O:
            return Key::O;
        case GLFW_KEY_P:
            return Key::P;
        case GLFW_KEY_Q:
            return Key::Q;
        case GLFW_KEY_R:
            return Key::R;
        case GLFW_KEY_S:
            return Key::S;
        case GLFW_KEY_T:
            return Key::T;
        case GLFW_KEY_U:
            return Key::U;
        case GLFW_KEY_V:
            return Key::V;
        case GLFW_KEY_W:
            return Key::W;
        case GLFW_KEY_X:
            return Key::X;
        case GLFW_KEY_Y:
            return Key::Y;
        case GLFW_KEY_Z:
            return Key::Z;

        case GLFW_KEY_SPACE:
            return Key::Space;
        case GLFW_KEY_ENTER:
            return Key::Enter;
        case GLFW_KEY_ESCAPE:
            return Key::Escape;
        case GLFW_KEY_TAB:
            return Key::Tab;
        case GLFW_KEY_BACKSPACE:
            return Key::Backspace;

        default:
            if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
                return static_cast<Key>(static_cast<int>(Key::Num0) + key -
                                        GLFW_KEY_0);
            }

            if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F24) {
                return static_cast<Key>(static_cast<int>(Key::F1) + key -
                                        GLFW_KEY_F1);
            }

            return Key::Unknown;
        }
    }

    constexpr MouseButton FromGLFWMouseButton(int button) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return MouseButton::Left;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return MouseButton::Middle;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return MouseButton::Right;
        case GLFW_MOUSE_BUTTON_4:
            return MouseButton::Button4;
        case GLFW_MOUSE_BUTTON_5:
            return MouseButton::Button5;
        case GLFW_MOUSE_BUTTON_6:
            return MouseButton::Button6;
        case GLFW_MOUSE_BUTTON_7:
            return MouseButton::Button7;
        case GLFW_MOUSE_BUTTON_8:
            return MouseButton::Button8;
        default:
            return MouseButton::Unknown;
        }
    }
}

namespace sal::backend {
    void GLFWInput::Initialize() {
        if (_loaded) {
            Terminate();
        }

        // check window
        GLFWWindow &window = static_cast<GLFWWindow &>(GetWindow());

        if (!window.IsInitialized()) {
            Console::Error("Input => Failed to initialize GLFW input because "
                           "the window is not loaded");
            return;
        }

        GLFWwindow *windowHandle =
            static_cast<GLFWwindow *>(window.GetHandle());

        // callbacks
        glfwSetKeyCallback(windowHandle, [](GLFWwindow *window, int key,
                                            int scan, int action, int mods) {
            GLFWInput &inputManager = static_cast<GLFWInput &>(
                static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window))
                    ->GetInputManager());

            Key target = FromGLFWKey(key);
            bool pressing = action != GLFW_RELEASE;

            inputManager._pressingKeys[static_cast<SizeT>(target)] = pressing;

            InputStateEvent e;
            e.Type = InputType::Keyboard;
            e.State = pressing ? InputState::Begin : InputState::End;
            e.Key = target;

            if (pressing) {
                inputManager.InputBegin.Fire(e);
            } else {
                inputManager.InputEnded.Fire(e);
            }

            inputManager.InputChanged.Fire(e);
        });

        glfwSetMouseButtonCallback(
            windowHandle,
            [](GLFWwindow *window, int button, int action, int mods) {
                GLFWInput &inputManager = static_cast<GLFWInput &>(
                    static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window))
                        ->GetInputManager());

                MouseButton target = FromGLFWMouseButton(button);
                bool pressing = action != GLFW_RELEASE;

                inputManager._pressingMouseButtons[static_cast<SizeT>(target)] =
                    pressing;

                InputStateEvent e;
                e.Position = inputManager.GetMousePosition();
                e.Type = InputType::Mouse;
                e.State = pressing ? InputState::Begin : InputState::End;
                e.MouseButton = target;

                if (pressing) {
                    inputManager.InputBegin.Fire(e);
                } else {
                    inputManager.InputEnded.Fire(e);
                }

                inputManager.InputChanged.Fire(e);
            });

        glfwSetScrollCallback(windowHandle, [](GLFWwindow *window,
                                               double xOffset, double yOffset) {
            GLFWInput &inputManager = static_cast<GLFWInput &>(
                static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window))
                    ->GetInputManager());

            inputManager._mouseScroll = Vector2(xOffset, yOffset);

            InputStateEvent e;
            e.Type = InputType::MouseScroll;
            e.Delta = inputManager._mouseScroll;

            inputManager.InputChanged.Fire(e);
        });

        _loaded = true;

        // reset data
        _pressingKeys.fill(false);
        _previousPressingKeys.fill(false);
        _pressingMouseButtons.fill(false);
        _previousPressingMouseButtons.fill(false);

        double xPos, yPos;
        glfwGetCursorPos(windowHandle, &xPos, &yPos);
        _mousePosition = Vector2(xPos, yPos);
        _lastMousePosition = _mousePosition;
        _mouseDelta = Vector2::Zero;
        _mouseScroll = Vector2::Zero;
        SetMouseState(_mouseState);
    }

    void GLFWInput::Terminate() {
        if (!_loaded) {
            return;
        }

        // reset stats
        _pressingKeys.fill(false);
        _previousPressingKeys.fill(false);
        _pressingMouseButtons.fill(false);
        _previousPressingMouseButtons.fill(false);
        _mousePosition = Vector2(0, 0);
        _lastMousePosition = _mousePosition;
        _mouseDelta = Vector2::Zero;
        _mouseScroll = Vector2::Zero;

        // disconnect callbacks
        GLFWWindow &window = static_cast<GLFWWindow &>(GetWindow());

        if (!window.IsInitialized()) {
            _loaded = false;
            return;
        }

        GLFWwindow *windowHandle =
            static_cast<GLFWwindow *>(window.GetHandle());

        // callbacks
        glfwSetKeyCallback(windowHandle, nullptr);
        glfwSetMouseButtonCallback(windowHandle, nullptr);
        glfwSetScrollCallback(windowHandle, nullptr);

        _loaded = false;
    }

    void GLFWInput::SimulateInput(MouseButton button, InputState state) {
        if (state == InputState::Change) {
            return;
        }

        _pressingMouseButtons[static_cast<SizeT>(button)] =
            state == InputState::Begin;

        InputStateEvent e;
        e.Type = InputType::Mouse;
        e.MouseButton = button;
        e.State = state;

        if (state == InputState::Begin) {
            InputBegin.Fire(e);
        } else {
            InputEnded.Fire(e);
        }

        InputChanged.Fire(e);
    }

    void GLFWInput::SimulateInput(Key key, InputState state) {
        if (state == InputState::Change) {
            return;
        }

        _pressingKeys[static_cast<SizeT>(key)] = state == InputState::Begin;

        InputStateEvent e;
        e.Type = InputType::Keyboard;
        e.Key = key;
        e.State = state;

        if (state == InputState::Begin) {
            InputBegin.Fire(e);
        } else {
            InputEnded.Fire(e);
        }

        InputChanged.Fire(e);
    }

    void GLFWInput::SetMouseState(MouseState state) {
        _mouseState = state;

        if (!_loaded) {
            return;
        }

        // disconnect callbacks
        GLFWWindow &window = static_cast<GLFWWindow &>(GetWindow());

        if (!window.IsInitialized()) {
            return;
        }

        GLFWwindow *windowHandle =
            static_cast<GLFWwindow *>(window.GetHandle());

        GLenum targetState = GLFW_CURSOR_NORMAL;

        switch (state) {
        case sal::MouseState::Hidden:
            targetState = GLFW_CURSOR_HIDDEN;
            break;
        case sal::MouseState::Disabled:
            targetState = GLFW_CURSOR_DISABLED;
            break;
        default:
            targetState = GLFW_CURSOR_NORMAL;
            break;
        }

        glfwSetInputMode(windowHandle, GLFW_CURSOR, targetState);
    }

    void GLFWInput::SetMousePosition(Vector2 position) {
        if (!_loaded) {
            return;
        }

        // disconnect callbacks
        GLFWWindow &window = static_cast<GLFWWindow &>(GetWindow());

        if (!window.IsInitialized()) {
            return;
        }

        GLFWwindow *windowHandle =
            static_cast<GLFWwindow *>(window.GetHandle());

        glfwSetCursorPos(windowHandle, position.X, position.Y);
    }

    void GLFWInput::OnUpdate() {
        if (!_loaded) {
            return;
        }

        // disconnect callbacks
        GLFWWindow &window = static_cast<GLFWWindow &>(GetWindow());

        if (!window.IsInitialized()) {
            return;
        }

        GLFWwindow *windowHandle =
            static_cast<GLFWwindow *>(window.GetHandle());

        // update keys
        _previousPressingKeys = _pressingKeys;
        _previousPressingMouseButtons = _pressingMouseButtons;

        // update mouse
        double xPos, yPos;
        glfwGetCursorPos(windowHandle, &xPos, &yPos);
        Vector2 newPos = Vector2(xPos, yPos);
        Vector2 lastPos = _mousePosition;

        _mousePosition = newPos;
        _lastMousePosition = lastPos;
        _mouseDelta = newPos - lastPos;

        // update scroll
        _mouseScroll = Vector2::Zero;
    }
}
