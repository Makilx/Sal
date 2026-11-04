#include <SalEngine/Services/WindowService.hpp>
#include <SalEngine/Services/DataModel.hpp>
#include <SalEngine/Instances/3D/Part3D.hpp>
#include <SalEngine/Instances/3D/Workspace3D.hpp>
#include <SalInterface/Renderer/IRenderer.hpp>

namespace sal {
    namespace {
        using Vertex = interface::Vertex;
        using Vertices = Vector<Vertex>;
        using Indices = Vector<unsigned int>;
        using PrimalMeshData = std::pair<Vertices, Indices>;

        void AddVertex(
            Vertices &vertices,
            Vector3 position,
            Vector3 normal,
            Vector2 uv) {
            vertices.push_back({position, normal, uv});
        }

        void AddTriangle(
            Indices &indices,
            unsigned int a,
            unsigned int b,
            unsigned int c) {
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
        }

        void AddQuad(
            Vertices &vertices,
            Indices &indices,
            Vector3 a,
            Vector3 b,
            Vector3 c,
            Vector3 d,
            Vector3 normal) {
            unsigned int start = static_cast<unsigned int>(vertices.size());

            AddVertex(vertices, a, normal, {0.0f, 0.0f});
            AddVertex(vertices, b, normal, {1.0f, 0.0f});
            AddVertex(vertices, c, normal, {1.0f, 1.0f});
            AddVertex(vertices, d, normal, {0.0f, 1.0f});

            AddTriangle(indices, start, start + 1, start + 2);
            AddTriangle(indices, start, start + 2, start + 3);
        }

        Vector3 Normalize(Vector3 value) {
            float length = std::sqrt(
                value.X * value.X +
                value.Y * value.Y +
                value.Z * value.Z);

            if (length <= 0.000001f) {
                return {0.0f, 1.0f, 0.0f};
            }

            return {
                value.X / length,
                value.Y / length,
                value.Z / length};
        }

        PrimalMeshData GetBlock() {
            Vertices vertices;
            Indices indices;

            AddQuad(vertices, indices,
                    {-0.5f, -0.5f, 0.5f},
                    {0.5f, -0.5f, 0.5f},
                    {0.5f, 0.5f, 0.5f},
                    {-0.5f, 0.5f, 0.5f},
                    {0.0f, 0.0f, 1.0f});

            AddQuad(vertices, indices,
                    {0.5f, -0.5f, -0.5f},
                    {-0.5f, -0.5f, -0.5f},
                    {-0.5f, 0.5f, -0.5f},
                    {0.5f, 0.5f, -0.5f},
                    {0.0f, 0.0f, -1.0f});

            AddQuad(vertices, indices,
                    {0.5f, -0.5f, 0.5f},
                    {0.5f, -0.5f, -0.5f},
                    {0.5f, 0.5f, -0.5f},
                    {0.5f, 0.5f, 0.5f},
                    {1.0f, 0.0f, 0.0f});

            AddQuad(vertices, indices,
                    {-0.5f, -0.5f, -0.5f},
                    {-0.5f, -0.5f, 0.5f},
                    {-0.5f, 0.5f, 0.5f},
                    {-0.5f, 0.5f, -0.5f},
                    {-1.0f, 0.0f, 0.0f});

            AddQuad(vertices, indices,
                    {-0.5f, 0.5f, 0.5f},
                    {0.5f, 0.5f, 0.5f},
                    {0.5f, 0.5f, -0.5f},
                    {-0.5f, 0.5f, -0.5f},
                    {0.0f, 1.0f, 0.0f});

            AddQuad(vertices, indices,
                    {-0.5f, -0.5f, -0.5f},
                    {0.5f, -0.5f, -0.5f},
                    {0.5f, -0.5f, 0.5f},
                    {-0.5f, -0.5f, 0.5f},
                    {0.0f, -1.0f, 0.0f});

            return {vertices, indices};
        }

        PrimalMeshData GetWedge() {
            Vertices vertices;
            Indices indices;

            Vector3 a = {-0.5f, -0.5f, -0.5f};
            Vector3 b = {0.5f, -0.5f, -0.5f};
            Vector3 c = {0.5f, 0.5f, -0.5f};
            Vector3 d = {-0.5f, -0.5f, 0.5f};
            Vector3 e = {0.5f, -0.5f, 0.5f};
            Vector3 f = {0.5f, 0.5f, 0.5f};

            AddQuad(vertices, indices, a, b, e, d, {0.0f, -1.0f, 0.0f});
            AddQuad(vertices, indices, b, c, f, e, {1.0f, 0.0f, 0.0f});

            unsigned int start = static_cast<unsigned int>(vertices.size());
            Vector3 slope = Normalize({-1.0f, 1.0f, 0.0f});

            AddVertex(vertices, a, slope, {0.0f, 0.0f});
            AddVertex(vertices, c, slope, {1.0f, 1.0f});
            AddVertex(vertices, f, slope, {0.0f, 1.0f});
            AddTriangle(indices, start, start + 1, start + 2);

            start = static_cast<unsigned int>(vertices.size());

            AddVertex(vertices, a, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f});
            AddVertex(vertices, b, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f});
            AddVertex(vertices, c, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f});
            AddTriangle(indices, start, start + 1, start + 2);

            start = static_cast<unsigned int>(vertices.size());

            AddVertex(vertices, d, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f});
            AddVertex(vertices, e, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f});
            AddVertex(vertices, f, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f});
            AddTriangle(indices, start, start + 1, start + 2);

            return {vertices, indices};
        }

        PrimalMeshData GetCornerWedge() {
            Vertices vertices;
            Indices indices;

            Vector3 p0 = {-0.5f, -0.5f, -0.5f};
            Vector3 p1 = {0.5f, -0.5f, -0.5f};
            Vector3 p2 = {-0.5f, 0.5f, -0.5f};
            Vector3 p3 = {-0.5f, -0.5f, 0.5f};
            Vector3 p4 = {0.5f, -0.5f, 0.5f};
            Vector3 p5 = {-0.5f, 0.5f, 0.5f};

            AddQuad(vertices, indices, p0, p1, p4, p3, {0.0f, -1.0f, 0.0f});
            AddQuad(vertices, indices, p0, p3, p5, p2, {-1.0f, 0.0f, 0.0f});

            unsigned int start = static_cast<unsigned int>(vertices.size());
            Vector3 slope = Normalize({1.0f, 1.0f, -1.0f});

            AddVertex(vertices, p1, slope, {0.0f, 0.0f});
            AddVertex(vertices, p2, slope, {1.0f, 0.0f});
            AddVertex(vertices, p5, slope, {1.0f, 1.0f});
            AddVertex(vertices, p4, slope, {0.0f, 1.0f});

            AddTriangle(indices, start, start + 1, start + 2);
            AddTriangle(indices, start, start + 2, start + 3);

            return {vertices, indices};
        }

        PrimalMeshData GetBall() {
            Vertices vertices;
            Indices indices;

            constexpr unsigned int segments = 32;
            constexpr unsigned int rings = 16;

            for (unsigned int y = 0; y <= rings; ++y) {
                float v = static_cast<float>(y) / rings;
                float phi = v * PI;

                for (unsigned int x = 0; x <= segments; ++x) {
                    float u = static_cast<float>(x) / segments;
                    float theta = u * PI * 2.0f;

                    Vector3 normal = {
                        std::sin(phi) * std::cos(theta),
                        std::cos(phi),
                        std::sin(phi) * std::sin(theta)};

                    AddVertex(vertices,
                              {normal.X * 0.5f,
                               normal.Y * 0.5f,
                               normal.Z * 0.5f},
                              normal,
                              {u, v});
                }
            }

            for (unsigned int y = 0; y < rings; ++y) {
                for (unsigned int x = 0; x < segments; ++x) {
                    unsigned int a = y * (segments + 1) + x;
                    unsigned int b = a + segments + 1;

                    AddTriangle(indices, a, b, a + 1);
                    AddTriangle(indices, a + 1, b, b + 1);
                }
            }

            return {vertices, indices};
        }

        PrimalMeshData GetCylinder() {
            Vertices vertices;
            Indices indices;

            constexpr unsigned int segments = 32;

            for (unsigned int i = 0; i <= segments; ++i) {
                float u = static_cast<float>(i) / segments;
                float angle = u * PI * 2.0f;

                float x = std::cos(angle);
                float z = std::sin(angle);

                Vector3 normal = {x, 0.0f, z};

                AddVertex(vertices,
                          {x * 0.5f, -0.5f, z * 0.5f},
                          normal,
                          {u, 0.0f});

                AddVertex(vertices,
                          {x * 0.5f, 0.5f, z * 0.5f},
                          normal,
                          {u, 1.0f});
            }

            for (unsigned int i = 0; i < segments; ++i) {
                unsigned int a = i * 2;

                AddTriangle(indices, a, a + 1, a + 2);
                AddTriangle(indices, a + 1, a + 3, a + 2);
            }

            unsigned int bottomCenter = static_cast<unsigned int>(vertices.size());
            AddVertex(vertices, {0.0f, -0.5f, 0.0f},
                      {0.0f, -1.0f, 0.0f}, {0.5f, 0.5f});

            unsigned int bottomStart = static_cast<unsigned int>(vertices.size());

            for (unsigned int i = 0; i <= segments; ++i) {
                float angle = static_cast<float>(i) / segments * PI * 2.0f;
                float x = std::cos(angle);
                float z = std::sin(angle);

                AddVertex(vertices,
                          {x * 0.5f, -0.5f, z * 0.5f},
                          {0.0f, -1.0f, 0.0f},
                          {x * 0.5f + 0.5f, z * 0.5f + 0.5f});
            }

            unsigned int topCenter = static_cast<unsigned int>(vertices.size());
            AddVertex(vertices, {0.0f, 0.5f, 0.0f},
                      {0.0f, 1.0f, 0.0f}, {0.5f, 0.5f});

            unsigned int topStart = static_cast<unsigned int>(vertices.size());

            for (unsigned int i = 0; i <= segments; ++i) {
                float angle = static_cast<float>(i) / segments * PI * 2.0f;
                float x = std::cos(angle);
                float z = std::sin(angle);

                AddVertex(vertices,
                          {x * 0.5f, 0.5f, z * 0.5f},
                          {0.0f, 1.0f, 0.0f},
                          {x * 0.5f + 0.5f, z * 0.5f + 0.5f});
            }

            for (unsigned int i = 0; i < segments; ++i) {
                AddTriangle(indices, bottomCenter,
                            bottomStart + i + 1, bottomStart + i);

                AddTriangle(indices, topCenter,
                            topStart + i, topStart + i + 1);
            }

            return {vertices, indices};
        }

        PrimalMeshData GetPrimalMeshData(Shape3D shape) {
            switch (shape) {
            case Shape3D::Block:
                return GetBlock();
            case Shape3D::Wedge:
                return GetWedge();
            case Shape3D::CornerWedge:
                return GetCornerWedge();
            case Shape3D::Ball:
                return GetBall();
            case Shape3D::Cylinder:
                return GetCylinder();
            default:
                return {};
            }
        }
    }
}

namespace sal {
    struct Part3D::Part3DHandle {
        interface::IMesh *CustomMesh = nullptr;
        interface::MaterialConfig Material;
    };

    void Workspace3D::OnInitialization(DataModel *owner) {
        InitializePrimalMeshes();
        Console::Print("Initializinig Workspace3D");
    }

    void Workspace3D::OnTerminate() {
        TerminatePrimalMeshes();

        _registeredParts.clear();
        _currentCamera = nullptr;
        Console::Print("Unitializinig Workspace3D");
    }

    void Workspace3D::OnDraw() {
        if (!IsInitialized()) {
            return;
        }

        interface::IRenderer *currentRenderer = static_cast<interface::IRenderer *>(GetRenderer());

        Dictionary<Shape3D, interface::RenderCommand> shapeCommands;

        shapeCommands[Shape3D::Ball] = {
            .Mesh = static_cast<interface::IMesh *>(GetPrimalMesh(Shape3D::Ball)),
            .Instanced = true,
        };

        shapeCommands[Shape3D::Block] = {
            .Mesh = static_cast<interface::IMesh *>(GetPrimalMesh(Shape3D::Block)),
            .Instanced = true,
        };

        shapeCommands[Shape3D::Wedge] = {
            .Mesh = static_cast<interface::IMesh *>(GetPrimalMesh(Shape3D::Wedge)),
            .Instanced = true,
        };

        shapeCommands[Shape3D::CornerWedge] = {
            .Mesh = static_cast<interface::IMesh *>(GetPrimalMesh(Shape3D::CornerWedge)),
            .Instanced = true,
        };

        shapeCommands[Shape3D::Cylinder] = {
            .Mesh = static_cast<interface::IMesh *>(GetPrimalMesh(Shape3D::Cylinder)),
            .Instanced = true,
        };

        // generate render commands for all parts
        for (Part3D *part : _registeredParts) {
            shapeCommands[part->GetShape()].Transforms.push_back(part->GetPosition().ToMatrix4().Scale(part->GetSize()));
        }

        Matrix4 view = CFrame::Identity.ToMatrix4();
        float fov = 90.0f;

        if (_currentCamera) {
            fov = _currentCamera->GetFieldOfView();
            view = _currentCamera->GetPosition().ToMatrix4();
        }

        WindowService &service = GetDataModel()->GetService<WindowService>();
        Matrix4 projection = Matrix4::Perspective(fov, (float)service.GetWindowSize().X / (float)service.GetWindowSize().Y, 0.1f, 100.0f);

        // render all
        currentRenderer->SetRenderView({
            .View = view,
            .Projection = projection,
        });

        for (auto command : shapeCommands) {
            currentRenderer->Draw(command.second);
        }
    }

    void Workspace3D::InitializePrimalMeshes() {
        auto *renderer = GetRenderer();
        if (renderer == nullptr) {
            return;
        }

        auto *createMesh = reinterpret_cast<interface::IRenderer *>(renderer);

        _primalMeshes.clear();

        for (Shape3D shape : {
                 Shape3D::Block,
                 Shape3D::Wedge,
                 Shape3D::CornerWedge,
                 Shape3D::Ball,
                 Shape3D::Cylinder}) {
            auto *mesh = createMesh->CreateMesh();

            if (mesh == nullptr) {
                continue;
            }

            // Geometry generation will be supplied by the shape builders.
            if (!mesh->Create()) {
                mesh->Destroy();
                delete mesh;
                continue;
            }

            PrimalMeshData data = GetPrimalMeshData(shape);

            mesh->SetVertices(data.first);
            mesh->SetIndices(data.second);

            _primalMeshes.emplace(shape, static_cast<void *>(mesh));
        }
    }

    void Workspace3D::TerminatePrimalMeshes() {
        for (auto &[shape, resource] : _primalMeshes) {
            auto *mesh = static_cast<interface::IMesh *>(resource);

            if (mesh == nullptr) {
                continue;
            }

            mesh->Destroy();
            delete mesh;
        }

        _primalMeshes.clear();
    }

    void *Workspace3D::GetPrimalMesh(Shape3D shape) {
        auto it = _primalMeshes.find(shape);

        if (it == _primalMeshes.end()) {
            return nullptr;
        }

        return it->second;
    }
}
