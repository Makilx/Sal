#pragma once

#include <SalEngine/Services/Service.hpp>

namespace sal {
    class InputService final : public Service {
    public:
        InputService(DataModel &dataModel, void *windowHandle)
            : Service(dataModel), _windowHandle(windowHandle) {}

        bool IsInitialized() const override { return _loaded; }
        String ToString() const override { return "InputService"; }

        Vector2 GetMousePosition() const;
        Vector2 GetMouseDelta() const;
        MouseState GetMouseState() const;

        bool GetKey(Key key) const;
        bool GetMouseButton(MouseButton button) const;
        bool GetKeyUp(Key key) const;
        bool GetMouseButtonUp(MouseButton button) const;
        bool GetKeyDown(Key key) const;
        bool GetMouseButtonDown(MouseButton button) const;

        void SetMousePosition(Vector2 position);
        void SetMouseState(MouseState state);
        void SimulateKey(Key key, InputState action);
        void SimulateMouseButton(MouseButton button, InputState action);

        bool ActionExists(const String &action) const {
            return _actions.contains(action);
        }

        bool GetAction(const String &action) const {
            auto it = _currentActionStates.find(action);
            return it != _currentActionStates.end() && it->second;
        }

        bool GetActionUp(const String &action) const {
            auto current = _currentActionStates.find(action);
            auto previous = _previousActionStates.find(action);
            return !(current != _currentActionStates.end() &&
                     current->second) &&
                   (previous != _previousActionStates.end() &&
                    previous->second);
        }

        bool GetActionDown(const String &action) const {
            auto current = _currentActionStates.find(action);
            auto previous = _previousActionStates.find(action);
            return (current != _currentActionStates.end() && current->second) &&
                   !(previous != _previousActionStates.end() &&
                     previous->second);
        }

        float GetActionAxis(const String &negativeAction,
                            const String &positiveAction) const {
            return ((GetAction(positiveAction) ? 1.0f : 0.0f) -
                    (GetAction(negativeAction) ? 1.0f : 0.0f));
        }

        void SetAction(const String &actionName,
                       std::initializer_list<ActionInput> inputs) {
            _actions[actionName] = Vector<ActionInput>(inputs);
            _currentActionStates[actionName] = false;
            _previousActionStates[actionName] = false;
        }

        void DeleteAction(const String &action) {
            _actions.erase(action);
            _currentActionStates.erase(action);
            _previousActionStates.erase(action);
        }

    public:
        Signal<InputStateEvent> InputBegin;
        Signal<InputStateEvent> InputEnded;
        Signal<InputStateEvent> InputChanged;
        Signal<MouseStateChangedEvent> MouseStateChanged;

    private:
        bool Initialize() override;
        void Terminate() override;

    private:
        bool _loaded = false;
        void *_windowHandle = nullptr;

        Dictionary<String, Vector<ActionInput>> _actions;
        Dictionary<String, bool> _currentActionStates;
        Dictionary<String, bool> _previousActionStates;
    };
}
