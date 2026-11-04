#include <SalEngine/Instances/3D/Part3D.hpp>
#include <SalInterface/Renderer/IMesh.hpp>
#include <SalInterface/Renderer/MaterialConfig.hpp>
#include <SalInterface/Renderer/ITexture.hpp>

namespace sal {
    struct Part3D::Part3DHandle {
        interface::IMesh *CustomMesh = nullptr;
        interface::MaterialConfig Material;
    };

    Part3D::Part3DHandle Part3D::GetHandle() const {
        return {
            .CustomMesh = nullptr,
            .Material = {
                .Shaders = nullptr,
                .Texture = nullptr,
                .CustomMaterial = _material,
            }};
    }
}
