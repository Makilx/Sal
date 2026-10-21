#pragma once

namespace sal::interface {
    class IRenderResource {
    public:
        virtual ~IRenderResource() = default;

        virtual void Destroy() = 0;
        virtual bool IsCreated() const = 0;
    };
}
