#pragma once

#include <SalEngine/Services/Service.hpp>

namespace sal {
    class RunService;

    class RunConnection {
    public:
        void Disconnect();
        bool IsConnected() const;

        RunService *GetService() const { return _service; }

    private:
        RunService *_service = nullptr;
        SizeT _connectionID = 0;

        friend class RunService;
    };

    class RunService final : public Service {
        using Listener = Function<void(double)>;

    public:
        RunService(DataModel &dataModel, void *windowHandle);

        bool IsInitialized() const override { return _loaded; }
        String ToString() const override { return "RunService"; }

        int GetMaxFramerate() const { return _maxFramerate; }
        void SetMaxFramerate(int framerate) { _maxFramerate = framerate; }
        int GetFramerate() const { return _currentFramerate; }

        float GetRunSpeed() const { return _runSpeed; }
        void SetRunSpeed(float speed) { _runSpeed = speed; }

        double GetRealTime() const { return _realTime; }
        double GetTime() const { return _time; }

        double GetRealDeltaTime() const { return _realDeltaTime; }
        double GetDeltaTime() const { return _deltaTime; }

        float GetPhysicsSpeed() const { return _physicsSpeed; }
        void SetPhysicsSpeed(float speed) { _physicsSpeed = speed; }

        bool IsVSYNCEnabled() const;
        void SetVSYNCEnabled(bool enabled);

        RunConnection BindToUpdate(SizeT priority, Listener listener) {
            return CreateConnection(priority, listener);
        }

        template <typename O>
        RunConnection BindToUpdate(SizeT priority, O *object,
                                   void (O::*listener)(double)) {
            return CreateConnection(priority, object, listener);
        }

    private:
        bool Initialize() override;
        void Terminate() override;

        void Step();

    private:
        struct ConnectionData {
            SizeT _priority;
            bool _connected;
            Listener _listener;
        };

        void DisconnectConnection(RunConnection &connection) {
            for (ConnectionData &data : _boundConnections) {
                if (data._priority == connection._connectionID) {
                    data._connected = false;
                    return;
                }
            }
        }

        bool IsConnectionValid(const RunConnection &connection) const {
            for (const ConnectionData &con : _boundConnections) {
                if (con._priority == connection._connectionID) {
                    return con._connected;
                }
            }
            return false;
        }

        RunConnection CreateConnection(SizeT priority, Listener listener) {
            SizeT id = priority;

            while (std::any_of(_boundConnections.begin(),
                               _boundConnections.end(),
                               [id](const ConnectionData &connection) {
                                   return connection._priority == id;
                               })) {
                ++id;
            }

            _boundConnections.push_back({id, true, listener});

            RunConnection newConnection;
            newConnection._service = this;
            newConnection._connectionID = id;

            return newConnection;
        }

        template <typename O>
        RunConnection CreateConnection(SizeT priority, O *object,
                                       void (O::*listener)(double)) {
            Listener callback = [object, listener](double deltaTime) {
                (object->*listener)(deltaTime);
            };

            return CreateConnection(priority, std::move(callback));
        }

    private:
        bool _loaded = false;
        void *_windowHandle = nullptr;

        int _maxFramerate = 60;
        int _currentFramerate = 0;

        double _time = 0;
        double _deltaTime = 0;
        double _realTime = 0;
        double _realDeltaTime = 0;
        double _lastRealTime = 0;

        double _lastFrameTime = 0.0;
        double _fpsUpdateTime = 0.0;
        SizeT _fpsFrameCount = 0;

        float _runSpeed = 1;
        float _physicsSpeed = 1;

        double _startTime = 0;

        Vector<ConnectionData> _boundConnections;

        friend class DataModel;
        friend class RunConnection;
    };

    inline void RunConnection::Disconnect() {
        if (!IsConnected()) {
            return;
        }

        _service->DisconnectConnection(*this);
    }

    inline bool RunConnection::IsConnected() const {
        return _service != nullptr && _service->IsConnectionValid(*this);
    }
}
