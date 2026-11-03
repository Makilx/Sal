#include <SalEngine/Instances/Workspace.hpp>

namespace sal {
    void Workspace::Initialize(DataModel *owner) {
        if (_loaded) {
            return;
        }

        _dataModel = owner;
        OnInitialization(owner);

        _loaded = true;
    }

    void Workspace::Terminate() {
        if (!_loaded) {
            return;
        }

        OnTerminate();
        _dataModel = nullptr;
        _loaded = false;
    }

    void Workspace::RegisterInstance(Instance *instance) {
        auto it = std::find(_registeredInstances.begin(), _registeredInstances.end(), instance);

        if (it != _registeredInstances.end()) {
            return;
        }

        instance->_owner = this;
        _registeredInstances.push_back(instance);

        OwnershipChangedEvent oce;
        oce.Owner = this;
        instance->OwnershipChanged.Fire(oce);
    }

    void Workspace::UnregisterInstance(Instance *instance) {
        auto it = std::find(_registeredInstances.begin(), _registeredInstances.end(), instance);

        if (it == _registeredInstances.end()) {
            return;
        }

        instance->_owner = nullptr;
        _registeredInstances.erase(it);

        OwnershipChangedEvent oce;
        oce.Owner = nullptr;
        instance->OwnershipChanged.Fire(oce);
    }
}
