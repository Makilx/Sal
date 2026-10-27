#pragma once

#include <SalCore/Core.hpp>

namespace sal {
    class Instance {
    public:
        Instance();
        ~Instance();

        // stats
        String GetName() const;
        void SetName(const String &name);

        virtual String GetClassName() const { return "Instance"; }

        SizeT GetUniqueID() const;

        // parent
        Instance *GetParent() const;
        void SetParent(Instance *parent);

        // state
        void Destroy();

        // conversion
        template <typename T> requires(std::derived_from<T, Instance>)
        bool IsA() {
            if (std::is_same<T, Instance>()) {
                return true;
            }

            return As<T>() != nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        T *As() {
            if (std::is_same<T, Instance>()) {
                return this;
            }

            return dynamic_cast<T *>(this);
        }

        // get
        Vector<Instance *> GetChildren() const;
        Vector<Instance *> GetAncestors() const;
        Vector<Instance *> GetDescendants() const;

        // find
        Instance *FindFirstChild(const String &name) const;
        Instance *FindFirstAncestor(const String &name) const;
        Instance *FindFirstDescendant(const String &name) const;
        Instance *WaitForChild(const String &name, float timeout) const;

        template <typename T> requires(std::derived_from<T, Instance>)
        T *FindFirstChildWhichIsA() const {
            for (Instance *instance : GetChildren()) {
                if (T *converted = instance->As<T>() != nullptr) {
                    return converted;
                }
            }
            return nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        T *FindFirstAncestorWhichIsA() const {
            for (Instance *instance : GetAncestors()) {
                if (T *converted = instance->As<T>() != nullptr) {
                    return converted;
                }
            }
            return nullptr;
        }

        template <typename T> requires(std::derived_from<T, Instance>)
        T *FindFirstDescendantWichIsA() const {
            for (Instance *instance : GetDescendants()) {
                if (T *converted = instance->As<T>() != nullptr) {
                    return converted;
                }
            }
            return nullptr;
        }

    public:
    private:
        virtual bool OnChildAdded(Instance *child);
        virtual bool OnChildRemoved(Instance *child);
        virtual bool OnDestroy();

    private:
        SizeT _uniqueID;
        String _name;
        Instance *_parent;
        Vector<Instance *> _children;

        static SizeT _nextUniqueID;
    };
}
