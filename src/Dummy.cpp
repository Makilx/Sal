#include <SalCore/Core.hpp>
#include <SalBackend/Window/GLFWWindow.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLRenderer.hpp>

using namespace sal;

int main() {
    Console::Print("Starting up Sal Engine");

    Console::Print("Creating window");
    backend::GLFWWindow window;
    window.SetSize(IVector2(300, 400));
    window.SetTitle("Sal created window");
    window.Initialize();
    if (!window.IsInitialized()) {
        return 1;
    }
    Console::Print("Window created");

    // Fullscreen connection
    bool inFullscreen = false;
    window.GetInputManager().InputBegin.Connect(
        [&window, &inFullscreen](InputStateEvent &e) {
            Console::Print(e.Type, e.State, e.Key, e.MouseButton);
            if (e.Key == Key::F11) {
                inFullscreen = !inFullscreen;
                if (inFullscreen) {
                    window.SetState(WindowState::BorderlessFullscreen);
                } else {
                    window.SetState(WindowState::Windowed);
                }
            }
        });

    // Renderer
    Console::Print("Initializing renderer");
    backend::OpenGLRenderer renderer;
    renderer.Initialize(&window);
    if (!renderer.IsInitialized()) {
        window.Terminate();
        return 2;
    }
    Console::Print("Renderer was initialized");

    // Buffers
    String source;

    File::ReadAllText("resources/shader.vert", &source);
    interface::IShader *vert = renderer.CreateShader();
    vert->Create(sal::ShaderType::VertexShader, source);

    File::ReadAllText("resources/shader.frag", &source);
    interface::IShader *frag = renderer.CreateShader();
    frag->Create(sal::ShaderType::FragmentShader, source);

    interface::IShaderProgram *prog = renderer.CreateProgram();
    prog->Create();
    prog->Attach(*vert);
    prog->Attach(*frag);
    prog->Link();

    interface::IMesh *mesh = renderer.CreateMesh();
    Vector<interface::Vertex> vertices = {
        // Base: four corners, normal pointing down
        {{-0.5f, 0.0f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.0f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.0f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.0f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},

        // Front face
        {{-0.5f, 0.0f, 0.5f}, {0.0f, 0.4472f, 0.8944f}, {0.0f, 0.0f}},
        {{0.5f, 0.0f, 0.5f}, {0.0f, 0.4472f, 0.8944f}, {1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.4472f, 0.8944f}, {0.5f, 1.0f}},

        // Right face
        {{0.5f, 0.0f, 0.5f}, {0.8944f, 0.4472f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.0f, -0.5f}, {0.8944f, 0.4472f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.8944f, 0.4472f, 0.0f}, {0.5f, 1.0f}},

        // Back face
        {{0.5f, 0.0f, -0.5f}, {0.0f, 0.4472f, -0.8944f}, {0.0f, 0.0f}},
        {{-0.5f, 0.0f, -0.5f}, {0.0f, 0.4472f, -0.8944f}, {1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.4472f, -0.8944f}, {0.5f, 1.0f}},

        // Left face
        {{-0.5f, 0.0f, -0.5f}, {-0.8944f, 0.4472f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, 0.0f, 0.5f}, {-0.8944f, 0.4472f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {-0.8944f, 0.4472f, 0.0f}, {0.5f, 1.0f}}};
    Vector<unsigned int> indices = {// Base
                                    0, 2, 1, 0, 3, 2,

                                    // Four triangular sides
                                    4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    mesh->Create();
    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);

    interface::Material mat{
        .Shaders = prog,
        .Texture = nullptr,
        .Tint = Color(0.35, 0.3, 0.5),
    };

    CFrame pyramidPos = CFrame::Identity;
    interface::RenderCommand pyramidCommand{
        .Material = &mat,
        .Mesh = mesh,
        .Transforms = {pyramidPos.ToMatrix4()},
        .Instanced = false,
    };

    CFrame camera =
        CFrame{Vector3{0.0f, 0.0f, 3.0f}, Vector4{0.0f, 0.0f, 0.0f, 1.0f}};

    interface::RenderView view{
        .View = camera.Inverse().ToMatrix4(),
        .Projection = Matrix4::Perspective(
            80.0f, (float)window.GetSize().X / (float)window.GetSize().Y, 0.1f,
            100.0f)};

    renderer.SetRenderView(view);
    renderer.SetBackgroundColor(Color(0.15, 0.1, 0.2));

    while (!window.ShouldClose()) {
        window.PollEvents();

        view = {.View = camera.Inverse().ToMatrix4(),
                .Projection = Matrix4::Perspective(
                    80.0f,
                    (float)window.GetSize().X / (float)window.GetSize().Y, 0.1f,
                    100.0f)};
        renderer.SetRenderView(view);

        pyramidPos *= CFrame::FromEulerAnglesYXZ(0, .01, 0);
        pyramidCommand.Transforms.clear();
        pyramidCommand.Transforms.push_back(pyramidPos.ToMatrix4());

        renderer.BeginFrame();

        renderer.Draw(pyramidCommand);

        renderer.EndFrame();
        window.SwapBuffers();
    }

    return 0;
}
