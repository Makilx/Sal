#pragma once

#include <SalCore/Core.hpp>
#include <SalEngine/Services/Service.hpp>

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
        T *GetService() const {
            for (const UniquePtr<Service> &service : _loadedServices) {
                if (T *cast = dynamic_cast<T *>(service.get())) {
                    return cast;
                }
            }

            return nullptr;
        }

    private:
        void Initialize();
        void Terminate();

    private:
        bool _initialized = false;
        bool _running = false;
        Vector<UniquePtr<Service>> _loadedServices;
        int _targetFramerate = 144;
        void *_backendFactory = nullptr;

        // components
        struct ObjectHandles;
        UniquePtr<ObjectHandles> _handles;
    };
}
