#include <SalEngine/DataModel/DataModel.hpp>
#include <SalEngine/Services/WindowService.hpp>
#include <SalEngine/Services/RunService.hpp>
#include <SalBackend/BackendFactory.hpp>

namespace sal {
    struct DataModel::ObjectHandles {
        UniquePtr<interface::IWindow> _windowHandle = nullptr;
        UniquePtr<interface::IRenderer> _rendererHandle = nullptr;
        UniquePtr<interface::IAudioManager> _audioHandle = nullptr;
    };

    DataModel::DataModel()
        : _handles(std::make_unique<DataModel::ObjectHandles>()) {
        // register services
        Initialize();
    }

    DataModel::~DataModel() { Terminate(); }

    bool DataModel::Run() {
        if (_running) {
            return true;
        }

        if (!_initialized) {
            return false;
        }

        _running = true;

        // initialize services
        Console::Break();
        Console::Divider("Service initialization");
        for (UniquePtr<Service> &service : _loadedServices) {
            Console::Print("Initializing", service->ToString());
            bool success = service->Initialize();

            if (!success) {
                Console::Error("Failed to initialize ", service->ToString());
                Stop();
                return false;
            }

            Console::Success("Initialized", service->ToString(),
                             "successfully");
        }
        Console::Success("Engine started successfully");
        Console::Divider();
        Console::Break();

        // generate runtime loop
        interface::IWindow *window = _handles->_windowHandle.get();
        interface::IRenderer *renderer = _handles->_rendererHandle.get();

        RunService *runService = GetService<RunService>();

        renderer->Initialize(window);

        while (_running && !window->ShouldClose()) {
            // renderer
            runService->Step();
        }

        // on finish
        Stop();

        return true;
    }

    bool DataModel::IsRunning() { return _running; }

    void DataModel::Stop() {
        if (!_running) {
            return;
        }

        Console::Divider("Service termination");
        for (UniquePtr<Service> &service : _loadedServices) {
            Console::Print("Terminating service:", service->ToString());
            service->Terminate();
            Console::Success("Service terminated:", service->ToString());
        }

        Console::Print("Clearing services");
        _loadedServices.clear();
        Console::Success("Services cleared");
        Console::Divider();
        Console::Break();

        // terminate base components
        _handles->_rendererHandle->Terminate();
        _handles->_windowHandle->Terminate();
        _handles->_audioHandle->Terminate();

        _handles->_rendererHandle.reset();
        _handles->_windowHandle.reset();
        _handles->_audioHandle.reset();

        // set state
        _running = false;
    }

    void DataModel::Initialize() {
        if (_initialized) {
            return;
        }

        _initialized = true;

        // create backend factory
        _backendFactory = new backend::BackendFactory(
            RendererAPI::OpenGL, WindowAPI::GLFW, AudioAPI::MiniAudio);

        DataModel &dataModel = *this;

        // create components
        _handles->_windowHandle =
            static_cast<backend::BackendFactory *>(_backendFactory)
                ->CreateWindow();

        _handles->_rendererHandle =
            static_cast<backend::BackendFactory *>(_backendFactory)
                ->CreateRenderer();

        _handles->_audioHandle =
            static_cast<backend::BackendFactory *>(_backendFactory)
                ->CreateAudioManager();

        // register services
        _loadedServices.push_back(std::make_unique<RunService>(
            dataModel, _handles->_windowHandle.get()));

        _loadedServices.push_back(std::make_unique<WindowService>(
            dataModel, _handles->_windowHandle.get()));
    }

    void DataModel::Terminate() {
        if (!_initialized) {
            return;
        }

        if (_running) {
            Stop();
        }

        _initialized = false;
    }
}
