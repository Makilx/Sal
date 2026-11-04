#pragma once

#include <SalEngine/Instances/Instance.hpp>

namespace sal {
    class DataModel;

    class Workspace : public Instance {
    public:
        Workspace() : Instance("Workspace", nullptr) {}
        virtual ~Workspace() = default;

        // ownership
        Sal_PropertyG(DataModel *, DataModel, _dataModel);

        // lifetime
        bool IsInitialized() const { return _loaded; }

        // type
        String GetClassName() const override { return "Workspace"; }

    protected:
        Sal_PropertyG(Instances, RegisteredInstances, _registeredInstances);

        void *GetAudio() const;
        void *GetWindow() const;
        void *GetRenderer() const;

    private:
        bool _loaded = false;
        DataModel *_dataModel = nullptr;
        Instances _registeredInstances;

        void Initialize(DataModel *owner);
        void Terminate();

        void RegisterInstance(Instance *instance);
        void UnregisterInstance(Instance *instance);

        virtual void OnInitialization(DataModel *owner) = 0;
        virtual void OnTerminate() = 0;
        virtual void OnDraw() = 0;
        virtual void OnInstanceRegister(Instance *instance) {}
        virtual void OnInstanceUnregister(Instance *instance) {}

        friend class Instance;
        friend class DataModel;
    };
}
