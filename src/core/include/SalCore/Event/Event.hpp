#pragma once

#include <SalCore/Core.hpp>

namespace sal {
    struct Event {
    public:
        bool IsHandled() const { return _handled; }

        void MarkHandled() { _handled = true; }

        template <typename T>
            requires std::derived_from<T, Event>
        bool IsA() const {
            return dynamic_cast<const T *>(this) != nullptr;
        }

        virtual String ToString() const = 0;

    private:
        bool _handled = false;
    };

    struct BlankEvent : public Event {
    public:
        String ToString() const override { return "BlankEvent"; }
    };
}
