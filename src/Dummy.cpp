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

    dataModel.SetWorkspace(workspace);
    dataModel.Run();

    return 0;
}
