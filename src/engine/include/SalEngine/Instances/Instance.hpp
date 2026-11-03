#pragma once

#include <SalCore/Core.hpp>

#define Sal_PropertyG(type, name, built) \
    type Get##name() const { return built; }

#define Sal_PropertyGS(type, name, arg, built) \
    type Get##name() const { return built; }   \
    void Set##name(type arg) {                 \
        if (arg == built)                      \
            return;                            \
        built = arg;                           \
        PropertyChangedEvent e;                \
        e.Property = #name;                    \
        PropertyChanged.Fire(e);               \
    }

namespace sal {
    class Workspace;
    class Instance;

    using Instances = Vector<Instance *>;

    class Instance {
    public:
        Instance(const String &name = "Instance", Instance *parent = nullptr) : _name(name) { SetParent(parent); }
        virtual ~Instance() = default;

        // property
        Sal_PropertyGS(String, Name, name, _name);

        // ownership
        Sal_PropertyG(Workspace *, Owner, _owner);
        Sal_PropertyG(Instance *, Parent, _parent);
        Sal_PropertyG(Instances, Children, _children);
        Instances GetAncestors() const {
            Instances ancestors;

            Instance *current = _parent;

            while (current) {
                ancestors.push_back(current);
                current = current->_parent;
            }

            return ancestors;
        }
        Instances GetDescendants() const {
            Instances descendants;

            for (Instance *child : _children) {
                descendants.push_back(child);

                Instances childDescendants = child->GetDescendants();

                descendants.insert(
                    descendants.end(),
                    childDescendants.begin(),
                    childDescendants.end());
            }

            return descendants;
        }

        void SetParent(Instance *parent);
        void ClearAllChildren();

        // search
        Instance *FindFirstChild(const String &name) const {
            for (Instance *child : _children) {
                if (child->GetName() == name) {
                    return child;
                }
            }

            return nullptr;
        }
        Instance *FindFirstAncestor(const String &name) const {
            Instance *current = _parent;

            while (current) {
                if (current->GetName() == name) {
                    return current;
                }

                current = current->_parent;
            }

            return nullptr;
        }
        Instance *FindFirstDescendant(const String &name) const {
            for (Instance *child : _children) {
                if (child->GetName() == name) {
                    return child;
                }

                Instance *found = child->FindFirstDescendant(name);

                if (found) {
                    return found;
                }
            }

            return nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        T *FindFirstChildWhichIsA() {
            for (Instance *child : _children) {
                if (T *instance = child->As<T>()) {
                    return instance;
                }
            }

            return nullptr;
        }
        template <typename T> requires(std::derived_from<T, Instance>)
        T *FindFirstAncestorWhichIsA() {
            Instance *current = _parent;

            while (current) {
                if (T *instance = current->As<T>()) {
                    return instance;
                }

                current = current->_parent;
            }

            return nullptr;
        }
        template <typename T> requires(std::derived_from<T, Instance>)
        T *FindFirstDescendantWhichIsA() {
            for (Instance *child : _children) {
                if (T *instance = child->As<T>()) {
                    return instance;
                }

                if (T *instance = child->FindFirstDescendantWhichIsA<T>()) {
                    return instance;
                }
            }

            return nullptr;
        }

        bool HasChild(Instance *child) {
            if (!child) {
                return false;
            }

            return std::find(_children.begin(), _children.end(), child) != _children.end();
        }
        bool HasAncestor(Instance *ancestor) {
            if (!ancestor) {
                return false;
            }

            Instance *current = _parent;

            while (current) {
                if (current == ancestor) {
                    return true;
                }

                current = current->_parent;
            }

            return false;
        }
        bool HasDescendant(Instance *descendant) {
            if (!descendant) {
                return false;
            }

            for (Instance *child : _children) {
                if (child == descendant || child->HasDescendant(descendant)) {
                    return true;
                }
            }

            return false;
        }

        // type
        virtual String GetClassName() const { return "Instance"; }

        template <typename T> requires(std::derived_from<T, Instance>)
        bool IsA() const { return As<T>() != nullptr; }

        template <typename T> requires(std::derived_from<T, Instance>)
        T *As() {
            return dynamic_cast<T *>(this);
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        const T *As() const {
            return dynamic_cast<const T *>(this);
        }

        // lifetime + misc
        Instance *Clone();
        void Destroy();

        // events
        Signal<PropertyChangedEvent> PropertyChanged;
        Signal<OwnershipChangedEvent> OwnershipChanged;
        Signal<InstanceEvent> AncestryChanged;
        Signal<InstanceEvent> ChildAdded;
        Signal<InstanceEvent> ChildRemoved;

        // stringfy
        String ToString() const { return _name; }

        // global
        template <typename T> requires(std::derived_from<T, Instance> && !std::is_same_v<T, Instance>)
        static T *New() {
            if (IS_TERMINATING) {
                return nullptr;
            }
            UniquePtr<T> instance = std::make_unique<T>();
            T *ptr = instance.get();
            ALL_INSTANCES.push_back(std::move(instance));
            return ptr;
        }

        static void Terminate() {
            IS_TERMINATING = true;
            while (!ALL_INSTANCES.empty()) {
                Instance *instance = ALL_INSTANCES.back().get();
                instance->Destroy();

                if (!ALL_INSTANCES.empty() &&
                    ALL_INSTANCES.back().get() == instance) {
                    break;
                }
            }
        }

    private:
        String _name = "";

        Workspace *_owner = nullptr;
        Instance *_parent = nullptr;
        Instances _children;

        bool _destroying = false;
        bool _destroyed = false;

        static Vector<UniquePtr<Instance>> ALL_INSTANCES;
        static bool IS_TERMINATING;

        bool AddChild(Instance *child);
        bool RemoveChild(Instance *child);

        virtual bool OnDestroy() { return true; }
        virtual bool OnChildAdded(Instance *child) { return true; }
        virtual bool OnChildRemoved(Instance *child) { return true; }
        virtual Instance *OnClone() { return new Instance(_name); }

        friend class Workspace;
    };

    inline Vector<UniquePtr<Instance>> Instance::ALL_INSTANCES;
    inline bool Instance::IS_TERMINATING = false;

    Sal_Stringify_Prebuilt(Instance);
}
