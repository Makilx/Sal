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
        static_assert(std::is_base_of<Event, T>::value,
                      "Type T does not inherit base struct 'Event'");
        using Listener = Function<void(T &)>;

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
            // trigger listeners
            for (auto &connection : _connections) {
                if (!connection._connected) {
                    continue;
                }

                connection._listener(event);

                if (connection._once) {
                    connection._connected = false;
                }
            }

            // remove disconnected connections
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

        friend class Connection<T>;
    };
}