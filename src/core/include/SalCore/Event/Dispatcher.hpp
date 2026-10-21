#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Event/Event.hpp>

namespace sal {
    class Dispatcher;

    class DispatcherConnection {
    public:
        void Disconnect();
        bool IsConnected() const;

        bool IsOnce() const { return _once; }

        Dispatcher *GetDispatcher() const { return _dispatcher; }

    private:
        Dispatcher *_dispatcher = nullptr;
        SizeT _connectionID = 0;
        bool _once = false;

        friend class Dispatcher;
    };

    class Dispatcher {
        using Listener = Function<void(Event &)>;

    public:
        template <typename E>
        DispatcherConnection Connect(Function<void(E &listener)> listener) {
            return CreateConnection(false, listener);
        }

        template <typename E, typename O>
        DispatcherConnection Connect(O *object, void (O::*listener)(E &)) {
            return CreateConnection(false, object, listener);
        }

        template <typename E>
        DispatcherConnection ConnectOnce(Function<void(E &listener)> listener) {
            return CreateConnection(true, listener);
        }

        template <typename E, typename O>
        DispatcherConnection ConnectOnce(O *object, void (O::*listener)(E &)) {
            return CreateConnection(true, object, listener);
        }

        void Disconnect(DispatcherConnection &connection) {
            for (ConnectionData &data : _connections) {
                if (data._id == connection._connectionID) {
                    data._connected = false;
                    return;
                }
            }
        }

        void DisconnectAll() { _connections.clear(); }

        void Dispatch(Event &event) {
            for (ConnectionData &data : _connections) {
                if (!data._connected) {
                    continue;
                }

                data._listener(event);

                if (data._once) {
                    data._connected = false;
                }
            }

            // remove disconnected connections.
            for (auto it = _connections.begin(); it != _connections.end();) {
                if (!it->_connected) {
                    it = _connections.erase(it);
                } else {
                    ++it;
                }
            }
        }

    private:
        struct ConnectionData {
            SizeT _id;
            bool _once;
            bool _connected;
            Listener _listener;
        };

        template <typename E>
        DispatcherConnection CreateConnection(bool once,
                                              Function<void(E &)> listener) {
            Listener callback = [listener = std::move(listener)](Event &event) {
                if (!event.IsA<E>()) {
                    return;
                }

                listener(static_cast<E &>(event));
            };

            SizeT id = _connectionIndex++;

            _connections.push_back({id, once, true, std::move(callback)});

            DispatcherConnection connection;
            connection._dispatcher = this;
            connection._connectionID = id;
            connection._once = once;

            return connection;
        }

        template <typename E, typename O>
        DispatcherConnection CreateConnection(bool once, O *object,
                                              void (O::*listener)(E &)) {
            Listener callback = [object, listener](E &event) {
                (object->*listener)(event);
            };

            return CreateConnection(once, std::move(callback));
        }

        bool IsConnectionValid(const DispatcherConnection &connection) const {
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

        friend class DispatcherConnection;
    };

    inline void DispatcherConnection::Disconnect() {
        if (!IsConnected()) {
            return;
        }

        _dispatcher->Disconnect(*this);
    }

    inline bool DispatcherConnection::IsConnected() const {
        return _dispatcher != nullptr && _dispatcher->IsConnectionValid(*this);
    }
}