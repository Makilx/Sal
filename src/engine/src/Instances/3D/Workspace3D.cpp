#include <SalEngine/Services/WindowService.hpp>
#include <SalEngine/Services/DataModel.hpp>
#include <SalEngine/Services/RunService.hpp>
#include <SalEngine/Instances/3D/Part3D.hpp>
#include <SalEngine/Instances/3D/Workspace3D.hpp>
#include <SalInterface/Renderer/IRenderer.hpp>

namespace sal {
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

namespace sal {
    struct Part3D::Part3DHandle {
        interface::IShaderProgram *CustomShader = nullptr;
        interface::IMesh *CustomMesh = nullptr;
        interface::ITexture *Albedo = nullptr;
        Material Material;
    };
}

namespace sal {
    const String DEFAULT_VERTEX_SHADER_CODE = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec3 a_Normal;
        layout(location = 2) in vec2 a_TextureUV;

        layout(location = 3) in mat4 a_InstanceModel;
        layout(location = 7) in vec4 a_InstanceTint;
        layout(location = 8) in float a_InstanceMetallic;
        layout(location = 9) in float a_InstanceRoughness;
        layout(location = 10) in uint a_AlbedoLayer;

        uniform int Sal_Instanced;

        uniform mat4 Sal_Model;
        uniform mat4 Sal_View;
        uniform mat4 Sal_Projection;

        uniform vec4 Sal_Tint;
        uniform float Sal_Metallic;
        uniform float Sal_Roughness;

        out vec3 v_Normal;
        out vec3 v_FragPosition;
        out vec2 v_TextureUV;

        out vec4 v_Tint;
        out float v_Metallic;
        out float v_Roughness;

        flat out uint v_AlbedoLayer;

        void main() {
            mat4 model = Sal_Instanced != 0
                ? a_InstanceModel
                : Sal_Model;

            vec4 worldPosition = model * vec4(a_Position, 1.0);

            gl_Position = Sal_Projection * Sal_View * worldPosition;

            v_FragPosition = worldPosition.xyz;
            v_Normal = normalize(
                mat3(transpose(inverse(model))) * a_Normal
            );

            v_TextureUV = a_TextureUV;

            if (Sal_Instanced != 0) {
                v_Tint = a_InstanceTint;
                v_Metallic = a_InstanceMetallic;
                v_Roughness = a_InstanceRoughness;
                v_AlbedoLayer = a_AlbedoLayer;
            } else {
                v_Tint = Sal_Tint;
                v_Metallic = Sal_Metallic;
                v_Roughness = Sal_Roughness;
                v_AlbedoLayer = 0u;
            }
        }
    )";
    const String DEFAULT_FRAGMENT_SHADER_CODE = R"(
        #version 330 core

        in vec3 v_Normal;
        in vec3 v_FragPosition;
        in vec2 v_TextureUV;

        in vec4 v_Tint;
        in float v_Metallic;
        in float v_Roughness;

        flat in uint v_AlbedoLayer;

        uniform int Sal_Instanced;
        uniform int Sal_HasAlbedo;

        uniform sampler2D Sal_Albedo;
        uniform sampler2DArray Sal_AlbedoStack;

        uniform vec3 Sal_LightDirection;
        uniform vec3 Sal_LightColor;
        uniform vec3 Sal_AmbientColor;

        out vec4 FragColor;

        void main() {
            vec4 albedo = v_Tint;

            if (Sal_Instanced != 0) {
                albedo *= texture(
                    Sal_AlbedoStack,
                    vec3(v_TextureUV, float(v_AlbedoLayer))
                );
            } else if (Sal_HasAlbedo != 0) {
                albedo *= texture(Sal_Albedo, v_TextureUV);
            }

            vec3 normal = normalize(v_Normal);
            vec3 lightDirection = normalize(-Sal_LightDirection);
            vec3 viewDirection = normalize(-v_FragPosition);
            vec3 halfwayDirection = normalize(lightDirection + viewDirection);

            float diffuse = max(dot(normal, lightDirection), 0.0);

            float roughness = clamp(v_Roughness, 0.04, 1.0);
            float metallic = clamp(v_Metallic, 0.0, 1.0);

            float shininess = mix(128.0, 4.0, roughness);
            float specular = pow(
                max(dot(normal, halfwayDirection), 0.0),
                shininess
            );

            vec3 baseColor = albedo.rgb;

            vec3 diffuseColor = baseColor * (1.0 - metallic);
            vec3 specularColor = mix(
                vec3(0.04),
                baseColor,
                metallic
            );

            vec3 ambient = Sal_AmbientColor * baseColor;

            vec3 lighting =
                ambient +
                diffuseColor * Sal_LightColor * diffuse +
                specularColor * Sal_LightColor * specular;

            FragColor = vec4(lighting, albedo.a);
        }
    )";
}

namespace sal {
    void Workspace3D::OnInitialization(DataModel *owner) {
        InitializePrimalMeshes();
        Console::Print("Initializinig Workspace3D");

        interface::IRenderer *currentRenderer = static_cast<interface::IRenderer *>(GetRenderer());

        interface::IShader *vertShader = currentRenderer->CreateShader();
        vertShader->Create(ShaderType::VertexShader, DEFAULT_VERTEX_SHADER_CODE);

        interface::IShader *fragShader = currentRenderer->CreateShader();
        fragShader->Create(ShaderType::FragmentShader, DEFAULT_FRAGMENT_SHADER_CODE);

        interface::IShaderProgram *program = currentRenderer->CreateProgram();
        program->Create();
        program->Attach(*vertShader);
        program->Attach(*fragShader);
        program->Link();
        _defaultShaders = program;

        vertShader->Destroy();
        fragShader->Destroy();

        owner->GetService<RunService>().BindToUpdate(1000000, [currentRenderer](double) {
            interface::RenderStatus status = currentRenderer->GetRenderStatus();
            Console::Print(status.DrawCalls, status.InstancedDrawCalls);
        });
    }

    void Workspace3D::OnTerminate() {
        TerminatePrimalMeshes();

        _registeredParts.clear();
        _currentCamera = nullptr;
        static_cast<interface::IShaderProgram *>(_defaultShaders)->Destroy();
        Console::Print("Unitializinig Workspace3D");
    }

    void Workspace3D::OnDraw() {
        if (!IsInitialized()) {
            return;
        }

        interface::IRenderer *currentRenderer = static_cast<interface::IRenderer *>(GetRenderer());

        Vector<interface::RenderCommand> commands;

        for (Part3D *part : _registeredParts) {
            // check data
            Part3D::Part3DHandle handle = part->GetHandle();

            interface::IMesh *targetMesh = handle.CustomMesh ? handle.CustomMesh : static_cast<interface::IMesh *>(GetPrimalMesh(part->_shape));
            interface::IShaderProgram *program = handle.CustomShader ? handle.CustomShader : static_cast<interface::IShaderProgram *>(_defaultShaders);

            // view compactible commands
            bool compactible = false;
            for (interface::RenderCommand command : commands) {
                if (command.Shader == program && command.Mesh == targetMesh) {
                    // insert into and continue the run
                    compactible = true;
                    command.Instance.push_back({
                        .Model = part->GetPosition().ToMatrix4().Scale(part->GetSize()),
                        .Material = handle.Material,
                        .Albedo = handle.Albedo,
                    });
                }
            }

            if (compactible) {
                continue;
            }

            // create new command if no one found
            interface::RenderCommand newCommand;
            newCommand.Mesh = targetMesh;
            newCommand.Shader = program;
            newCommand.Instance.push_back({
                .Model = part->GetPosition().ToMatrix4().Scale(part->GetSize()),
                .Material = handle.Material,
                .Albedo = handle.Albedo,
            });

            commands.push_back(newCommand);
        }

        // set render view
        Vector2 windowSize = GetDataModel()->GetService<WindowService>().GetWindowSize().ToFloat();
        currentRenderer->SetRenderView({.View = (_currentCamera ? _currentCamera->GetPosition() : CFrame::Identity).ToMatrix4(),
                                        .Projection = Matrix4::Perspective(
                                            Deg(_currentCamera ? _currentCamera->GetFieldOfView() : 90.0f),
                                            windowSize.X / windowSize.Y,
                                            0.1f, 1000.0f)});

        // draw all commands
        for (interface::RenderCommand &command : commands) {
            currentRenderer->Draw(command);
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
