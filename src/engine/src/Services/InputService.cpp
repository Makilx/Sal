#include <SalEngine/Services/InputService.hpp>
#include <SalInterface/Input/IInput.hpp>
#include <SalInterface/Window/IWindow.hpp>
#include <SalEngine/DataModel/DataModel.hpp>
#include <SalEngine/Services/RunService.hpp>

namespace sal {
    Vector2 InputService::GetMousePosition() const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetMousePosition();
    }

    Vector2 InputService::GetMouseDelta() const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetMouseDelta();
    }

    MouseState InputService::GetMouseState() const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetMouseState();
    }

    bool InputService::GetKey(Key key) const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetInput(key);
    }

    bool InputService::GetMouseButton(MouseButton button) const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetInput(button);
    }

    bool InputService::GetKeyUp(Key key) const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetInputUp(key);
    }

    bool InputService::GetMouseButtonUp(MouseButton button) const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetInputUp(button);
    }

    bool InputService::GetKeyDown(Key key) const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetInputDown(key);
    }

    bool InputService::GetMouseButtonDown(MouseButton button) const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .GetInputDown(button);
    }

    void InputService::SetMousePosition(Vector2 position) {
        static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .SetMousePosition(position);
    }

    void InputService::SetMouseState(MouseState state) {
        static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .SetMouseState(state);
    }

    void InputService::SimulateKey(Key key, InputState action) {
        static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .SimulateInput(key, action);
    }

    void InputService::SimulateMouseButton(MouseButton button,
                                           InputState action) {
        static_cast<interface::IWindow *>(_windowHandle)
            ->GetInputManager()
            .SimulateInput(button, action);
    }

    bool InputService::Initialize() {
        if (_loaded) {
            return true;
        }

        // input connections
        interface::IInput &inputManager =
            static_cast<interface::IWindow *>(_windowHandle)->GetInputManager();

        inputManager.InputBegin.Connect(
            [this](InputStateEvent &e) { this->InputBegin.Fire(e); });

        inputManager.InputChanged.Connect(
            [this](InputStateEvent &e) { this->InputChanged.Fire(e); });

        inputManager.InputEnded.Connect(
            [this](InputStateEvent &e) { this->InputEnded.Fire(e); });

        inputManager.MouseStateChanged.Connect(
            [this](MouseStateChangedEvent &e) {
                this->MouseStateChanged.Fire(e);
            });

        // runservice connections
        GetDataModel().GetService<RunService>().BindToUpdate(
            static_cast<SizeT>(RunPriority::Input),
            [this, &inputManager](double) {
                // update action state
                for (std::pair<const String, Vector<ActionInput>> &action :
                     _actions) {
                    bool isPressing = false;

                    for (ActionInput input : action.second) {
                        isPressing = std::visit(
                            [&inputManager](auto value) {
                                return inputManager.GetInput(value);
                            },
                            input);

                        if (isPressing) {
                            break;
                        }
                    }

                    _currentActionStates[action.first] = isPressing;
                }

                // update previous
                _previousActionStates = _currentActionStates;
            });

        return true;
    }

    void InputService::Terminate() {
        if (!_loaded) {
            return;
        }

        // there is nothing to do here for now

        _loaded = false;
    }
}
