#include <SalEngine/Engine.hpp>

using namespace sal;

int main() {
    DataModel dataModel;

    WindowService &windowService = dataModel.GetService<WindowService>();
    windowService.SetWindowTitle("Sal Window");

    RunService &runService = dataModel.GetService<RunService>();
    runService.SetMaxFramerate(1);

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

    dataModel.Run();

    return 0;
}
