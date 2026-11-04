#pragma once

#include "SalCore/Misc/Enums.hpp"
#include "SalCore/Precompiled.hpp"
#include <SalEngine/Instances/Workspace.hpp>
#include <SalEngine/Instances/3D/Part3D.hpp>
#include <SalEngine/Instances/3D/Camera3D.hpp>

namespace sal {
    class Workspace3D : public Workspace {
    public:
        String GetClassName() const override { return "Workspace3D"; }

        Sal_PropertyGS(Camera3D *, CurrentCamera, camera, _currentCamera);

    private:
        Camera3D *_currentCamera = nullptr;
        Vector<Part3D *> _registeredParts;
        Dictionary<Shape3D, void *> _primalMeshes;
        void *_defaultShaders = nullptr;

        void OnInitialization(DataModel *owner) override;
        void OnTerminate() override;
        void OnDraw() override;

        void OnInstanceRegister(Instance *instance) override {
            Part3D *as = instance->As<Part3D>();
            if (as != nullptr) {
                _registeredParts.push_back(as);
            }
        }

        void OnInstanceUnregister(Instance *instance) override {
            Part3D *as = instance->As<Part3D>();
            if (as != nullptr) {
                _registeredParts.push_back(as);
            }
        }

        void *GetPrimalMesh(Shape3D shape);

        void InitializePrimalMeshes();
        void TerminatePrimalMeshes();
    };
}
