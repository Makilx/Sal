#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Event/Event.hpp>

namespace sal {
    template <typename T>
    class Signal;

    template <typename T>
    class Connection {
    public:
        void Disconnect() {
            if (!IsConnected()) {
                return;
            }

            _signal->Disconnect(*this);
        }

        bool IsConnected() const {
            return _signal != nullptr && _signal->IsConnectionValid(*this);
        }

        bool IsOnce() const { return _once; }

        Signal<T> *GetSignal() const { return _signal; }

    private:
        Signal<T> *_signal = nullptr;
        SizeT _connectionID = 0;
        bool _once = false;

        friend class Signal<T>;
    };

    template <typename T>
    class Signal {
        static_assert(std::is_base_of_v<Event, T>,
                      "Type T does not inherit base struct 'Event'");

        using Listener = Function<void(T &)>;

        struct Waiter {
            std::coroutine_handle<> _handle;
            std::shared_ptr<const T> _event;
            bool _active = true;
        };

        struct WaitState {
            Vector<std::shared_ptr<Waiter>> _waiters;
        };

    public:
        class WaitAwaitable {
        public:
            explicit WaitAwaitable(std::shared_ptr<WaitState> state)
                : _state(std::move(state)),
                  _waiter(std::make_shared<Waiter>()) {}

            WaitAwaitable(const WaitAwaitable &) = delete;
            WaitAwaitable &operator=(const WaitAwaitable &) = delete;

            WaitAwaitable(WaitAwaitable &&) noexcept = default;
            WaitAwaitable &operator=(WaitAwaitable &&) = delete;

            ~WaitAwaitable() {
                if (_waiter) {
                    _waiter->_active = false;
                }
            }

            bool await_ready() const noexcept {
                return false;
            }

            void await_suspend(std::coroutine_handle<> handle) {
                _waiter->_handle = handle;
                _state->_waiters.push_back(_waiter);
            }

            T await_resume() const {
                return *_waiter->_event;
            }

        private:
            std::shared_ptr<WaitState> _state;
            std::shared_ptr<Waiter> _waiter;
        };

        WaitAwaitable Wait() {
            return WaitAwaitable(_waitState);
        }

    public:
        Connection<T> Connect(Listener listener) {
            return CreateConnection(false, listener);
        }

        template <typename O>
        Connection<T> Connect(O *object, void (O::*listener)(T &)) {
            return CreateConnection(false, object, listener);
        }

        Connection<T> ConnectOnce(Listener listener) {
            return CreateConnection(true, listener);
        }

        template <typename O>
        Connection<T> ConnectOnce(O *object, void (O::*listener)(T &)) {
            return CreateConnection(true, object, listener);
        }

        void Disconnect(Connection<T> &connection) {
            for (ConnectionData &data : _connections) {
                if (data._id == connection._connectionID) {
                    data._connected = false;
                    return;
                }
            }
        }

        void DisconnectAll() { _connections.erase(); }

        void Fire(T &event) {
            Vector<std::shared_ptr<Waiter>> waiters;
            waiters.swap(_waitState->_waiters);

            // Trigger listeners
            for (auto &connection : _connections) {
                if (!connection._connected) {
                    continue;
                }

                connection._listener(event);

                if (connection._once) {
                    connection._connected = false;
                }
            }

            // Remove disconnected connections
            for (auto it = _connections.begin(); it != _connections.end();) {
                if (!it->_connected) {
                    it = _connections.erase(it);
                } else {
                    ++it;
                }
            }

            // Create one event copy for all waiting coroutines
            std::shared_ptr<const T> sharedEvent;

            for (const auto &waiter : waiters) {
                if (!waiter->_active) {
                    continue;
                }

                if (!sharedEvent) {
                    sharedEvent = std::make_shared<T>(event);
                }

                waiter->_event = sharedEvent;
                waiter->_active = false;
                waiter->_handle.resume();
            }
        }

    private:
        struct ConnectionData {
            SizeT _id;
            bool _connected;
            bool _once;
            Listener _listener;
        };

        Connection<T> CreateConnection(bool once, Listener listener) {
            SizeT id = _connectionIndex++;
            _connections.push_back({id, true, once, listener});

            Connection<T> newConnection;
            newConnection._signal = this;
            newConnection._connectionID = id;
            newConnection._once = once;

            return newConnection;
        }

        template <typename O>
        Connection<T> CreateConnection(bool once, O *object,
                                       void (O::*listener)(T &)) {
            Listener callback = [object, listener](T &event) {
                (object->*listener)(event);
            };

            return CreateConnection(once, std::move(callback));
        }

        bool IsConnectionValid(const Connection<T> &connection) const {
            for (const ConnectionData &con : _connections) {
                if (con._id == connection._connectionID) {
                    return con._connected;
                }
            }
            return false;
        }

    private:
        Vector<ConnectionData> _connections;
        SizeT _connectionIndex = 0;
        std::shared_ptr<WaitState> _waitState = std::make_shared<WaitState>();

        friend class Connection<T>;
    };
}
