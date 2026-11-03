#pragma once

#include <SalEngine/Instances/Workspace.hpp>

namespace sal {
    class Workspace3D : public Workspace {
    public:
        String GetClassName() const override { return "Workspace3D"; }

    private:
        virtual void OnInitialization(DataModel *owner) override {
            Console::Print("Workspace initialize with datamodel", owner);
        }

        virtual void OnTerminate() override {
            Console::Print("Workspace is terminating");
        }

        virtual void OnDraw() override {
            Console::Warn("Draw operations enter here");
        }
    };
}
