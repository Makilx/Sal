#include <SalEngine/Instances/Instance.hpp>
#include <SalEngine/Instances/Workspace.hpp>

namespace sal {
    void Instance::SetParent(Instance *parent) {
        if (_destroying || _destroyed || _parent == parent || (parent && HasDescendant(parent))) {
            return;
        }

        if (parent && (parent->_destroying || parent->_destroyed)) {
            return;
        }

        if (parent && !parent->AddChild(this)) {
            return;
        }

        Instance *oldParent = _parent;
        Workspace *oldOwner = _owner;

        if (oldParent) {
            oldParent->RemoveChild(this);
        }

        if (oldOwner) {
            oldOwner->UnregisterInstance(this);
        }

        _parent = parent;

        Workspace *currentOwner = parent
                                      ? (parent->IsA<Workspace>() ? parent->As<Workspace>() : parent->_owner)
                                      : nullptr;

        _owner = currentOwner;

        if (_owner) {
            _owner->RegisterInstance(this);
        }

        InstanceEvent ace;
        ace.Instance = parent;
        AncestryChanged.Fire(ace);

        PropertyChangedEvent pce;
        pce.Property = "Parent";
        PropertyChanged.Fire(pce);

        for (Instance *descendant : GetDescendants()) {
            Workspace *previousOwner = descendant->_owner;

            if (previousOwner) {
                previousOwner->UnregisterInstance(descendant);
            }

            descendant->_owner = currentOwner;

            if (currentOwner) {
                currentOwner->RegisterInstance(descendant);
            }

            OwnershipChangedEvent descendantEvent;
            descendantEvent.Owner = currentOwner;
            descendant->OwnershipChanged.Fire(descendantEvent);

            InstanceEvent descendantAncestry;
            descendantAncestry.Instance = parent;
            descendant->AncestryChanged.Fire(descendantAncestry);
        }
    }

    void Instance::ClearAllChildren() {
        while (!_children.empty()) {
            Instance *child = _children.back();
            SizeT previousSize = _children.size();

            child->Destroy();

            if (_children.size() == previousSize) {
                break;
            }
        }
    }

    Instance *Instance::Clone() {
        return OnClone();
    }

    void Instance::Destroy() {
        if (_destroying || _destroyed) {
            return;
        }

        _destroying = true;

        if (!OnDestroy()) {
            _destroying = false;
            return;
        }

        ClearAllChildren();

        if (!_children.empty()) {
            _destroying = false;
            return;
        }

        SetParent(nullptr);

        if (_owner) {
            _owner->UnregisterInstance(this);
            _owner = nullptr;
        }

        _destroyed = true;
        _destroying = false;

        auto it = std::find_if(
            ALL_INSTANCES.begin(),
            ALL_INSTANCES.end(),
            [this](const UniquePtr<Instance> &instance) {
                return instance.get() == this;
            });

        if (it != ALL_INSTANCES.end()) {
            ALL_INSTANCES.erase(it);
        }
    }

    bool Instance::AddChild(Instance *child) {
        if (!child || HasChild(child) || child->_destroying || child->_destroyed) {
            return false;
        }

        if (!OnChildAdded(child)) {
            return false;
        }

        _children.push_back(child);

        InstanceEvent cae;
        cae.Instance = child;
        ChildAdded.Fire(cae);

        return true;
    }

    bool Instance::RemoveChild(Instance *child) {
        if (!child) {
            return false;
        }

        auto it = std::find(_children.begin(), _children.end(), child);

        if (it == _children.end() || !OnChildRemoved(child)) {
            return false;
        }

        _children.erase(it);

        InstanceEvent cae;
        cae.Instance = child;
        ChildRemoved.Fire(cae);

        return true;
    }
}
