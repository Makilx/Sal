#include <SalEngine/Engine.hpp>

using namespace sal;

int main() {
    DataModel dataModel;

    String name = Console::Read("Please enter the window name");

    int width;
    int height;
    String output1 = Console::Read("Enter the window width");
    String output2 = Console::Read("Enter the window width");
    bool success1 = StrToInt(output1, &width);
    bool success2 = StrToInt(output2, &height);

    if (!success1 || !success2) {
        return 1;
    }

    Console::Warn(name, width, height);

    WindowService *windowService = dataModel.GetService<WindowService>();
    windowService->SetWindowTitle(name);
    windowService->SetWindowSize(IVector2(width, height));

    RunService *runService = dataModel.GetService<RunService>();
    runService->SetMaxFramerate(1);

    runService->BindToUpdate(5000, [runService](double deltaTime) {
        Console::Print(runService->GetFramerate());
    });

    Console::Print("Starting the engine!");
    dataModel.Run();
    Console::Print("Yay!");

    return 0;
}
