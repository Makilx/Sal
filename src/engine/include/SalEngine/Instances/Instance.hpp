#pragma once

#include <SalCore/Core.hpp>

#define Sal_Property_GSetter(type, name, param, builtin) \
    type Get##name() const { return builtin; }           \
    void Set##name(type param) {                         \
        this->builtin = param;                           \
        PropertyChangedEvent e;                          \
        e.Property = #name;                              \
        PropertyChanged.Fire(e);                         \
    }

namespace sal {
    class Workspace;
    class Instance;

    using Instances = Vector<Instance *>;

    class Instance {
    public:
        Instance();
        virtual ~Instance() = default;

        virtual String GetClassName() const { return "Instance"; }
        String ToString() const { return GetName(); }

        void SetName(const String &name);
        void SetParent(Instance *parent);

        SizeT GetUniqueID() const { return _uniqueID; }
        String GetName() const { return _name; }
        Workspace *GetOwner() const { return _owner; }

        Instance *GetParent() const { return _parent; }
        Instances GetChildren() const { return _children; }
        Instances GetAncestors() const;
        Instances GetDescendants() const;

        Instance *FindFirstChild(const String &name) const {
            for (Instance *target : _children) {
                if (target->_name == name) {
                    return target;
                }
            }

            return nullptr;
        }

        Instance *FindFirstAncestor(const String &name) const {
            Instance *current = _parent;

            while (current) {
                if (current->_name == name) {
                    return current;
                }

                current = current->_parent;
            }

            return nullptr;
        }

        Instance *FindFirstDescendant(const String &name) const {
            for (Instance *target : _children) {
                if (target->_name == name) {
                    return target;
                }

                if (Instance *descendant = target->FindFirstDescendant(name)) {
                    return descendant;
                }
            }

            return nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance> && !std::is_same_v<T, Instance>)
        T *FindFirstChildWhichIsA() const {
            for (Instance *inst : _children) {
                if (T *target = inst->As<T>()) {
                    return target;
                }
            }

            return nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance> && !std::is_same_v<T, Instance>)
        T *FindFirstAncestorWhichIsA() const {
            Instance *current = _parent;

            while (current) {
                if (T *target = current->As<T>()) {
                    return target;
                }

                current = current->_parent;
            }

            return nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance> && !std::is_same_v<T, Instance>)
        T *FindFirstDescendantWhichIsA() const {
            for (Instance *inst : _children) {
                if (T *target = inst->As<T>()) {
                    return target;
                }

                if (T *target = inst->FindFirstDescendantWhichIsA<T>()) {
                    return target;
                }
            }

            return nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        bool IsA() const {
            return As<T>() != nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        T *As() {
            return dynamic_cast<T *>(this);
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        const T *As() const {
            return dynamic_cast<const T *>(this);
        }

        Instance *Clone();

        void Destroy();

    public:
        Signal<PropertyChangedEvent> PropertyChanged;
        Signal<InstanceEvent> AncestryChanged;
        Signal<InstanceEvent> ChildAdded;
        Signal<InstanceEvent> ChildRemoved;
        Signal<BlankEvent> Destroying;

    private:
        virtual bool OnChildAdded(Instance *child) { return true; }
        virtual bool OnChildRemoved(Instance *child) { return true; }
        virtual bool OnDestroy() { return true; }
        virtual Instance *OnClone() { return nullptr; }

        bool AddChild(Instance *child);
        bool RemoveChild(Instance *child);

    private:
        String _name = "";
        SizeT _uniqueID = 0;
        Instances _children;
        Instance *_parent = nullptr;
        Workspace *_owner = nullptr;

        static SizeT ID_COUNT;
    };
}
