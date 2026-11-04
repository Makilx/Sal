#pragma once

#include "SalEngine/Instances/3D/Workspace3D.hpp"
#include <SalCore/Core.hpp>
#include <SalEngine/Services/Service.hpp>
#include <SalEngine/Instances/Workspace.hpp>

namespace sal {
    class DataModel {
    public:
        DataModel();
        ~DataModel();

        bool Run();
        bool IsRunning();
        void Stop();

        template <typename T>
        requires(std::is_base_of_v<Service, T> &&
                 !std::is_same_v<Service, T>)
        T &GetService() const {
            for (const UniquePtr<Service> &service : _loadedServices) {
                if (T *cast = dynamic_cast<T *>(service.get())) {
                    return *cast;
                }
            }

            throw std::logic_error(
                "DataModel invariant violated: service not found.");
        }

        TaskScheduler &Task() { return _sheduler; }

        Sal_PropertyG(Workspace *, Workspace, _workspace);
        void SetWorkspace(Workspace *workspace);

        String ToString() const { return "DataModel"; }

    private:
        bool _initialized = false;
        bool _running = false;

        void *_backendFactory = nullptr;

        Vector<UniquePtr<Service>> _loadedServices;

        // workspace
        Workspace *_workspace = nullptr;

        // scheduler
        TaskScheduler _sheduler;

        // components
        struct ObjectHandles;
        UniquePtr<ObjectHandles> _handles;

        void Initialize();
        void Terminate();

        void *GetAudio() const;
        void *GetWindow() const;
        void *GetRenderer() const;

        friend class Workspace;
    };

    Sal_Stringify_Prebuilt(DataModel);
}
