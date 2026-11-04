#include <SalEngine/Instances/3D/Part3D.hpp>
#include <SalInterface/Interface.hpp>

namespace sal {
    struct Part3D::Part3DHandle {
        interface::IShaderProgram *CustomShader = nullptr;
        interface::IMesh *CustomMesh = nullptr;
        interface::ITexture *Albedo = nullptr;
        Material Material;
    };
}

namespace sal {
    Part3D::Part3DHandle Part3D::GetHandle() const {
        return {
            .CustomMesh = nullptr,
            .Albedo = nullptr,
            .Material = _material,
        };
    }
}
