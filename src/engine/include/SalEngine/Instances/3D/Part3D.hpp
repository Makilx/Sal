#pragma once

#include <SalEngine/Instances/Instance.hpp>

namespace sal {
    class Part3D : public Instance {
    public:
        Sal_PropertyGS(CFrame, Position, position, _position);
        Sal_PropertyGS(Vector3, Size, size, _size);
        Sal_PropertyGS(Shape3D, Shape, shape, _shape);
        Sal_PropertyGS(Color, Color, color, _material.Tint);
        Sal_PropertyGS(float, Metallic, metallic, _material.Metallic);
        Sal_PropertyGS(float, Roughness, roughness, _material.Roughness);

    private:
        Vector3 _size = Vector3::One;
        Shape3D _shape = Shape3D::Block;
        CFrame _position = CFrame::Identity;
        Material _material;

        struct Part3DHandle;
        Part3DHandle GetHandle() const;

        friend class Workspace3D;
    };
}
