#include "SalCore/IO/Console.hpp"
#include <SalEngine/Services/DataModel.hpp>
#include <SalEngine/Services/WindowService.hpp>
#include <SalEngine/Services/RunService.hpp>
#include <SalEngine/Services/InputService.hpp>
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

        RunService &runService = GetService<RunService>();

        renderer->Initialize(window);
        renderer->SetBackgroundColor(Color(0.5, 0.2, 0.3));

        // bind runservice renderer
        auto &scheduler = _sheduler;

        runService.BindToUpdate(static_cast<SizeT>(RunPriority::First), [&scheduler, &runService](double deltaTime) {
            scheduler.Update(runService.GetRealDeltaTime());
        });

        runService.BindToUpdate(static_cast<SizeT>(RunPriority::Update),
                                [renderer](double) { renderer->BeginFrame(); });

        runService.BindToUpdate(static_cast<SizeT>(RunPriority::Update) + 1,
                                [this](double) { if (_workspace){ _workspace->OnDraw(); } });

        runService.BindToUpdate(static_cast<SizeT>(RunPriority::Render) - 1,
                                [renderer](double) { renderer->EndFrame(); });

        // workspace
        if (_workspace) {
            _workspace->Initialize(this);
        }

        while (_running && !window->ShouldClose()) {
            // update
            runService.Step();
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

        // workspace
        if (_workspace) {
            _workspace->Terminate();
        }

        Console::Break();
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

        _loadedServices.push_back(std::make_unique<InputService>(
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
        Console::Success("Terminating datamodel");
    }

    void *DataModel::GetAudio() const {
        return _handles->_audioHandle.get();
    }

    void *DataModel::GetWindow() const {
        return _handles->_windowHandle.get();
    }

    void *DataModel::GetRenderer() const {
        return _handles->_rendererHandle.get();
    }

    void DataModel::SetWorkspace(Workspace *workspace) {
        if (_running) {
            if (_workspace) {
                _workspace->Terminate();
            }

            _workspace = workspace;

            if (_workspace) {
                _workspace->Initialize(this);
            }
        } else {
            _workspace = workspace;
        }
    }
}
