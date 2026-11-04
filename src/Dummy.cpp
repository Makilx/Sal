#include <SalEngine/Engine.hpp>

using namespace sal;

int main() {
    DataModel dataModel;

    WindowService &windowService = dataModel.GetService<WindowService>();
    windowService.SetWindowTitle("Sal Window");

    RunService &runService = dataModel.GetService<RunService>();
    runService.SetMaxFramerate(0);

    InputService &inputService = dataModel.GetService<InputService>();
    bool inFullscreen = false;
    inputService.InputBegin.Connect([&windowService,
                                     &inFullscreen](InputStateEvent &e) {
        if (e.Key == Key::F11) {
            inFullscreen = !inFullscreen;
            windowService.SetWindowState(inFullscreen
                                             ? WindowState::BorderlessFullscreen
                                             : WindowState::Windowed);
        }
    });

    Workspace3D *workspace = Instance::New<Workspace3D>();

    Part3D *part = Instance::New<Part3D>();
    part->SetParent(workspace);

    Camera3D *camera = Instance::New<Camera3D>();
    workspace->SetCurrentCamera(camera);
    camera->SetPosition(camera->GetPosition() * CFrame(0, 0, 2));

    dataModel.SetWorkspace(workspace);
    dataModel.Run();

    return 0;
}
