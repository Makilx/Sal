#pragma once

#include <SalEngine/Instances/Instance.hpp>

namespace sal {
    class Camera3D : public Instance {
    public:
        Sal_PropertyGS(CFrame, Position, position, _position);
        Sal_PropertyGS(float, FieldOfView, fov, _fieldOfView);

    private:
        CFrame _position = CFrame::Identity;
        float _fieldOfView = 90.0f;
    };
}
