#include <SalEngine/Services/RunService.hpp>
#include <SalEngine/DataModel/DataModel.hpp>
#include <SalEngine/Services/WindowService.hpp>
#include <SalInterface/Window/IWindow.hpp>

double GetTimeDouble() {
    using namespace std::chrono;
    return duration<double>(steady_clock::now().time_since_epoch()).count();
}

namespace sal {
    RunService::RunService(DataModel &dataModel, void *windowHandle)
        : Service(dataModel), _windowHandle(windowHandle) {}

    RunService::~RunService() { Terminate(); }

    bool RunService::IsVSYNCEnabled() const {
        return static_cast<interface::IWindow *>(_windowHandle)
            ->IsVSYNCEnabled();
    }

    void RunService::SetVSYNCEnabled(bool enabled) {
        static_cast<interface::IWindow *>(_windowHandle)->SetVSYNC(enabled);
    }

    bool RunService::Initialize() {
        if (_loaded) {
            return true;
        }

        _startTime = GetTimeDouble();
        _lastRealTime = 0;
        _realTime = 0;
        _realDeltaTime = 0;
        _time = 0;
        _deltaTime = 0;

        _lastFrameTime = 0.0;
        _fpsUpdateTime = _startTime;
        _fpsFrameCount = 0;
        _currentFramerate = 0;

        _loaded = true;
        return true;
    }

    void RunService::Terminate() {
        if (!_loaded) {
            return;
        }

        // remove all connections
        _boundConnections.clear();
        _loaded = false;
    }

    void RunService::Step() {
        if (!_loaded) {
            return;
        }

        if (_maxFramerate > 0 && !IsVSYNCEnabled() && _lastFrameTime > 0.0) {
            const double targetFrameTime = 1.0 / _maxFramerate;
            const double elapsed = GetTimeDouble() - _lastFrameTime;

            if (elapsed < targetFrameTime) {
                std::this_thread::sleep_for(
                    std::chrono::duration<double>(targetFrameTime - elapsed));
            }
        }

        const double currentTime = GetTimeDouble();

        _realTime = currentTime - _startTime;
        _realDeltaTime =
            _lastFrameTime > 0.0 ? currentTime - _lastFrameTime : 0.0;

        _lastFrameTime = currentTime;

        _deltaTime = _realDeltaTime * _runSpeed;
        _time += _deltaTime;

        for (ConnectionData &connection : _boundConnections) {
            if (!connection._connected) {
                continue;
            }

            connection._listener(_deltaTime);
        }

        for (auto it = _boundConnections.begin();
             it != _boundConnections.end();) {
            if (!it->_connected) {
                it = _boundConnections.erase(it);
            } else {
                ++it;
            }
        }

        ++_fpsFrameCount;

        const double fpsElapsed = currentTime - _fpsUpdateTime;

        if (fpsElapsed >= 1.0) {
            _currentFramerate = static_cast<int>(_fpsFrameCount / fpsElapsed);

            _fpsFrameCount = 0;
            _fpsUpdateTime = currentTime;
        }
    }
}
